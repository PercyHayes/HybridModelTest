#include "./../include/Weight_Loader.h"

void Weight_Loader(
    packed_io_int4* weight_mem,                           // weights from HBM, 512bits
    hls::stream<packed_io_int4>& weight_stream,          // weights to PE array, 512bits
    const int offset,                                      // offset to adjust address
    const int Iterations                                 // number of iterations
){
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        packed_io_int4 weight_data = weight_mem[iter + offset];
        weight_stream.write(weight_data);
    }

}