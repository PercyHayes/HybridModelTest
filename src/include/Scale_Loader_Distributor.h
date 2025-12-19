#ifndef __SCALE_LOADER_DISTRIBUTOR_H__
#define __SCALE_LOADER_DISTRIBUTOR_H__

#include "./../common/params.h"

/**
 * @brief Load scales from HBM memory and distribute to PE array
 * 
 * @param scale_mem Scales from HBM memory, packed format
 * @param scale_stream Output scale streams to PE array
 * @param offset Offset to adjust address
 * @param Iterations Number of iterations
 */
void Scale_Loader_Distributor(
    packed_io_scale* scale_mem,
    hls::stream<scale_t> scale_stream[NUM_LANES],
    const int offset,  
    const int Iterations
);

#endif // __SCALE_LOADER_DISTRIBUTOR_H__