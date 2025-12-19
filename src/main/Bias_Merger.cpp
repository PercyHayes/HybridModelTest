#include "./../include/Bias_Merger.h"


void Bias_Merger(
    hls::stream<activation_t> in_stream[NUM_LANES],
    hls::stream<packed_io_activation>& out_stream,
    activation_t *bias_buffer_onchip,
    const int offset,
    const int rows
){
    #ifndef __SYNTHESIS__
    assert(rows % NUM_LANES == 0 && "rows must be multiple of NUM_LANES");
    #endif
    for(int i = 0; i < rows; i++){
        #pragma HLS LOOP_TRIPCOUNT min=896 max=896
        #pragma HLS PIPELINE II=1
        packed_io_activation packed_data = 0;
        // read bias values from NUM_LANES streams and pack them
        for(int lane = 0; lane < NUM_LANES; lane++){
            #pragma HLS UNROLL
            activation_t bias_value = in_stream[lane].read();
            activation_t bias = bias_buffer_onchip[offset + i * NUM_LANES + lane];
            //converter_t merged_result;
            //merged_result.f = bias_value + bias;
            activation_t merged_result = bias_value + bias;
            packed_data.range((lane + 1) * ACTIVATION_BITS - 1, lane * ACTIVATION_BITS) = *reinterpret_cast<ap_uint<ACTIVATION_BITS>*>(&merged_result);
            //static_cast<ap_uint<ACTIVATION_BITS>>(merged_result);
            #ifndef __SYNTHESIS__
            if(i == 0){
                std::cout << "Bias_Merger row " << i << ", lane " << lane << ": bias_value = " << bias_value << ", bias = " << bias << ", merged_result = " << merged_result << std::endl;
            }
            #endif
        }
        // write packed bias data to output stream
        out_stream.write(packed_data);

    }
}