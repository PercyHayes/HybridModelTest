#include "./../include/Accumulator_Quantizer.h"


void Accumulator_Quantizer(
    hls::stream<acc_t>& in_stream,
    hls::stream<scale_t>& scale_stream,
    hls::stream<activation_t>& out_stream,
    const int rows,
    const int cols
){
    for(int r = 0; r < rows; r++){
        #pragma HLS LOOP_TRIPCOUNT min=896 max=896

        acc_t acc_result = 0.0;
        //activation_t acc_tmp[2] = {0.0, 0.0};
        //#pragma HLS ARRAY_PARTITION variable=acc_tmp complete
        for(int c = 0; c < cols; c++){
            #pragma HLS LOOP_TRIPCOUNT min=28 max=28
            #pragma HLS PIPELINE II=1
            //#pragma HLS UNROLL factor=2
            // read input activation
            acc_t acc_value = in_stream.read();
            // read scale value
            scale_t scale_value = scale_stream.read();

            
            // apply scaling and accumulation
            acc_result += (acc_value * scale_value);
            #pragma HLS BIND_OP variable=acc_result op=mul impl=dsp

            #ifndef __SYNTHESIS__
                if(r == 1 && c == 27){
                    std::cout << "acc_value: " << acc_value << ", scale_value: " << float(scale_value) << "acc result: " << float(acc_result) << std::endl;
                }
            #endif
            //acc_tmp += (acc_value * scale_value);
//#pragma HLS BIND_OP variable=acc_tmp op=hmul impl=dsp
            
            
        }

        // write output activation
        //activation_t acc_result = acc_tmp[0] + acc_tmp[1];

        out_stream.write(acc_result.to_half());
    }
}