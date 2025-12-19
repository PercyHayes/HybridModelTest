#ifndef __BIAS_MERGER_H__
#define __BIAS_MERGER_H__

#include "./../common/params.h"

/**
 * @brief Merge bias values from on-chip buffer and output to stream
 * 
 * @param in_stream           input activation streams from NUM_LANES lanes
 * @param out_stream          output activation stream after merging bias, packed to 512 bits
 * @param bias_buffer_onchip  on-chip buffer storing bias values
 * @param offset              offset to adjust bias address
 * @param rows                number of output rows
 */

void Bias_Merger(
    hls::stream<activation_t> in_stream[NUM_LANES],
    hls::stream<packed_io_activation>& out_stream,
    activation_t *bias_buffer_onchip,
    const int offset,
    const int rows
);


#endif // __BIAS_MERGER_H__