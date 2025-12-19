#ifndef __PARAMS_H__
#define __PARAMS_H__

// cpp standard
#include <stdint.h>
#include <assert.h>

// hls library
#include <hls_stream.h>
#include <hls_half.h>
#include <ap_fixed.h>
#include <ap_int.h>
#include <hls_vector.h>

const int NUM_LANES = 4;

const int QUANTIZATION_BITS = 4;
const int INP_ENTRIES = 128;

const int SCALE_BITS = 32;
const int SCALING_INP = 512 / SCALE_BITS;

const int ACTIVATION_BITS = 16;

const int ACTIVATION_INP = 512 / ACTIVATION_BITS;

typedef ap_int<QUANTIZATION_BITS> int4_t;
typedef ap_uint<QUANTIZATION_BITS * INP_ENTRIES> packed_io_int4;

typedef ap_uint<SCALE_BITS * SCALING_INP> packed_io_scale;
//typedef half scale_t;
typedef ap_fixed<32,16, AP_RND, AP_SAT> scale_t;

typedef ap_uint<ACTIVATION_BITS * NUM_LANES> packed_io_activation;

typedef ap_fixed<32,16, AP_RND, AP_SAT> acc_t;

//activation type and vector type
typedef half activation_t;
typedef hls::vector<activation_t, INP_ENTRIES> activation_vector_128;
typedef hls::vector<activation_t, 64> activation_vector_64;
typedef hls::vector<activation_t, 32> activation_vector_32;
typedef hls::vector<activation_t, 16> activation_vector_16;
typedef hls::vector<activation_t, 8> activation_vector_8;
typedef hls::vector<activation_t, 4> activation_vector_4;
typedef hls::vector<activation_t, 2> activation_vector_2;

// unuse union for type conversion between half(class) and ap_uint
typedef union {
	acc_t f;
    ap_uint<SCALE_BITS> i;
	//uint16_t i;
    // Add default constructor to handle non-trivial member 'half'
    //converter_t() : i(0) {} 
} converter_t;


#endif // __PARAMS_H__