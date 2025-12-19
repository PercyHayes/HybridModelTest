#ifndef __STREAM_COPY_H__
#define __STREAM_COPY_H__

#include "./../common/params.h"

/**
 * @brief Copy activation data from on-chip buffer to dispacher streams
 * 
 * @param input_buffer activation input buffer pointer
 * @param dispacher dispacher output stream array
 * @param rows number of rows
 * @param cols number of columns (original clomns / NUM_LANES)
 */
void Stream_Copy(
    activation_t *input_buffer,                                       // activation input buffer pointer
    hls::stream<activation_vector_128> dispacher[NUM_LANES],          // dispacher output stream array
    const int rows,                                                  // number of rows
    const int cols                                                  // number of columns (original clomns / NUM_LANES)
);
#endif // __STREAM_COPY_H__