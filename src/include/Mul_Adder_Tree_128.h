#ifndef __Mul_Adder_Tree_128_H__
#define __Mul_Adder_Tree_128_H__

#include "./../common/params.h"

/** 
* @brief perform multiplication and adder tree to get output activations
* 
* @param data_stream   input activation vector stream 128*16bits
* @param weight_stream input weight vector stream 128*4bits
* @param result_stream output activation stream 16bits
* @param Iterations    number of iterations
**/
void Mul_Adder_Tree_128(
    hls::stream<activation_vector_128>& data_stream,
    hls::stream<packed_io_int4>& weight_stream,
    hls::stream<acc_t>& result_stream,
    const int Iterations
);


#endif // __Mul_Adder_Tree_128_H__