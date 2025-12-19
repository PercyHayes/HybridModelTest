#ifndef __GEMV_TEST_H__
#define __GEMV_TEST_H__

#include "./../common/params.h"
#include "Mul_Adder_Tree_128.h"
#include "Weight_Loader.h"
#include "Stream_Copy.h"
#include "Scale_Loader_Distributor.h"
#include "Accumulator_Quantizer.h"
#include "Bias_Merger.h"

/**
 * @brief Top-level function to perform GEMV operation （for test demo)
 * 
 * @param weight_mem0       weight memory bank 0
 * @param weight_mem1       weight memory bank 1
 * @param weight_mem2       weight memory bank 2
 * @param weight_mem3       weight memory bank 3
 * @param scale_mem         scale memory for dequantization
 * @param out_stream        output activation stream
 * @param input_buffer      input activation buffer on chip
 * @param bias_buffer       bias buffer on chip
 * @param output_buffer     output activation buffer on chip （unused now)
 * @param rows              number of output rows
 * @param cols              number of input columns
 * @param weight_offset      offset for weight memory addressing
 * @param scale_offset       offset for scale memory addressing
 * @param bias_offset      offset for bias addressing
 */

void Gemv_Test(
    packed_io_int4* weight_mem0,
    packed_io_int4* weight_mem1,
    packed_io_int4* weight_mem2,
    packed_io_int4* weight_mem3,
    packed_io_scale* scale_mem,
    hls::stream<packed_io_activation>& out_stream,
    activation_t* input_buffer,
    activation_t* bias_buffer,
    activation_t* output_buffer,
    const int rows,
    const int cols,
    const int weight_offset,
    const int scale_offset,
    const int bias_offset
);


#endif // __GEMV_TEST_H__