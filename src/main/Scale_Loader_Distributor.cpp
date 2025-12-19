#include "./../include/Scale_Loader_Distributor.h"

/*
mem address:        [ 0  1  2  3  |  4  5  6  7  |  8  9 10 11  | 12 13 14 15 ] ...
corresponding PE:   [ P0 P1 P2 P3 | P0 P1 P2 P3 | P0 P1 P2 P3 | P0 P1 P2 P3 ] ...
                      ^             ^             ^             ^
                      Cycle 0       Cycle 1       Cycle 2       Cycle 3
Each memory word contains 32 scales (16 bits each), distributed to 4 PEs over 8 cycles.
*/



void Scale_Loader_Distributor(
    packed_io_scale* scale_mem,                             // scales from HBM, 512bits
    hls::stream<scale_t> scale_stream[NUM_LANES],           // scales to PE array, half
    const int offset,                                         // offset to adjust address
    const int Iterations                                    // number of iterations
){

    const int scales_per_word = 512 / SCALE_BITS;
    const int num_repeats = scales_per_word / NUM_LANES;

    load_from_hbm:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=6272 max=6272
        #pragma HLS PIPELINE II=8
        packed_io_scale scale_data = scale_mem[iter + offset];
        repeat:
        for(int repeat = 0; repeat < num_repeats; repeat++){
            #pragma HLS UNROLL
            distribute:
            for(int lane = 0; lane < NUM_LANES; lane++){
                #pragma HLS UNROLL
                scale_t data;
                data.range(SCALE_BITS - 1, 0) = scale_data.range((repeat * NUM_LANES + lane + 1) * SCALE_BITS - 1,
                                                                 (repeat * NUM_LANES + lane) * SCALE_BITS);
                
                /*scale_t data = scale_data.range((repeat * NUM_LANES + lane + 1) * SCALE_BITS - 1,
                                               (repeat * NUM_LANES + lane) * SCALE_BITS);*/
                
                scale_stream[lane].write(data);
            }
        }
        
    }


    /*for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=3136 max=3136
        #pragma HLS PIPELINE II=8
        packed_io_scale scale_data = scale_mem[iter + offset];
        repeat:
        for(int repeat = 0; repeat < num_repeats; repeat++){
            #pragma HLS UNROLL
            distribute:
            for(int lane = 0; lane < NUM_LANES; lane++){
                #pragma HLS UNROLL
                converter_t converter;
                converter.i = scale_data.range((repeat * NUM_LANES + lane + 1) * SCALE_BITS - 1,
                                               (repeat * NUM_LANES + lane) * SCALE_BITS);
                scale_stream[lane].write(converter.f);
            }
        }
        
    }*/
}