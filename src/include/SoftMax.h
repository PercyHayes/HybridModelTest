#ifndef __SOFTMAX_H__
#define __SOFTMAX_H__

#include "./../common/params.h"


/**
 * @brief apply softmax to input activation stream
 * 
 * @param in_stream  input activation stream
 * @param out_stream output activation stream after softmax
 * @param seq_len    length of the input sequence
 */
void Softmax(
    hls::stream<activation_t>& in_stream,
    hls::stream<activation_t>& out_stream,
    const int seq_len
);

#endif // __SOFTMAX_H__