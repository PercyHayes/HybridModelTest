#include "./../include/Stream_Copy.h"

void Stream_Copy(
    activation_t *input_buffer,                                       // activation input buffer pointer
    hls::stream<activation_vector_128> dispacher[NUM_LANES],          // dispacher output stream array
    const int rows,                                                  // number of rows
    const int cols                                                  // number of columns (original clomns / NUM_LANES)
){
    // copy from input buffer to dispacher streams
    row_loop:
    for(int r = 0; r < rows; r++){
        #pragma HLS LOOP_TRIPCOUNT min=896 max=896
        col_loop:
        for(int c = 0; c < cols; c++){
            #pragma HLS LOOP_TRIPCOUNT min=28 max=28
            #pragma HLS PIPELINE II=1
            #ifndef __SYNTHESIS__
            //std::cout << "Streaming row " << r << ", col " << c << std::endl;
            #endif
            // read data from input buffer
            activation_vector_128 data_vec;
            for(int i = 0; i < INP_ENTRIES; i++){
                #pragma HLS UNROLL
                data_vec[i] = input_buffer[c * INP_ENTRIES + i];
            }
            // write data to dispacher streams
            for(int lane = 0; lane < NUM_LANES; lane++){
                #pragma HLS UNROLL
                dispacher[lane].write(data_vec);
            }
        }
    }
}