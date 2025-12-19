#ifndef __WEIGHT_LOADER_H__
#define __WEIGHT_LOADER_H__

#include "./../common/params.h"

/**
 * @brief Load weights from HBM memory and stream to weight distributor
 * 
 * @param weight_mem Weights from HBM memory, packed format
 * @param weight_stream Output weight stream to weight distributor
 * @param offset Offset to adjust address
 * @param Iterations Number of iterations
 */
void Weight_Loader(
    packed_io_int4* weight_mem,
    hls::stream<packed_io_int4>& weight_stream,
    const int offset,
    const int Iterations
);

#endif // __WEIGHT_LOADER_H__