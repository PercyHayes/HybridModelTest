#ifndef __ACCUMULATOR_QUANTIZER_H__
#define __ACCUMULATOR_QUANTIZER_H__

#include "./../common/params.h"


/**
 * @brief apply scaling and accumulate adder tree to get final output activations
 * 
 * @param in_stream    input activation stream from Mul_Adder_Tree 
 * @param scale_stream input scale stream from scale_loader_distributor (group size = 128)
 * @param out_stream   output activation stream after scaling and dequantization
 * @param rows         number of output rows (original rows / NUM_LANES)
 * @param cols         number of input columns
 */
void Accumulator_Quantizer(
    hls::stream<acc_t>& in_stream,
    hls::stream<scale_t>& scale_stream,
    hls::stream<activation_t>& out_stream,
    const int rows,
    const int cols
);

#endif // __ACCUMULATOR_QUANTIZER_H__