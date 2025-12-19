#include "./../include/Gemv_Test.h"

/*
    test gemv function
    // test data sizes
    matrix size: 3584 * 3584
    vector size: 3584
    bias size: 3584
    scale size: 3584 * 28 (group size 128)
    output size: 3584

    // function arguments
    every weight_memX: point to weight matrix in HBM memory, packed format, read 25088 times (3584*3584/128/4)
    scale_mem: point to scale matrix in HBM memory, packed format(512bits), read 3136 times (3584*28/32)
    out_stream: output stream for final output activations, packed format (NUM_LANES * 16 bits), write 896 times (3584/4)
    input_buffer: input activation buffer on chip, format: half, size 3584, offset 0
    bias_buffer: bias buffer on chip, format: half, size 3584, offset bias_offset
    output_buffer: output activation buffer on chip, format: half, size 358
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
){
    #pragma HLS DATAFLOW

    // stream array for NUM_LANES lanes
    hls::stream<activation_vector_128> dispacher[NUM_LANES];
    #pragma HLS STREAM variable=dispacher depth=2
    #pragma HLS BIND_STORAGE variable=dispacher type=fifo impl=srl

    hls::stream<packed_io_int4> weight_streams[NUM_LANES];
    #pragma HLS STREAM variable=weight_streams depth=2
    #pragma HLS BIND_STORAGE variable=weight_streams type=fifo impl=srl

    hls::stream<acc_t> adder_tree_output[NUM_LANES];
    #pragma HLS STREAM variable=adder_tree_output depth=2
    #pragma HLS BIND_STORAGE variable=adder_tree_output type=fifo impl=srl

    hls::stream<scale_t> scale_dispacher[NUM_LANES];
    #pragma HLS STREAM variable=scale_dispacher depth=2
    #pragma HLS BIND_STORAGE variable=scale_dispacher type=fifo impl=srl

    hls::stream<activation_t> dequantized_output[NUM_LANES];
    #pragma HLS STREAM variable=dequantized_output depth=2
    #pragma HLS BIND_STORAGE variable=dequantized_output type=fifo impl=srl

    const int weight_read_iterations = (rows * cols) / (INP_ENTRIES * NUM_LANES); //25088 for 3584*3584 matrix

    const int scale_read_iterations = (rows * cols / INP_ENTRIES) / SCALING_INP; //6272

    // load weights and distribute to weight streams
    Weight_Loader(weight_mem0, weight_streams[0], weight_offset, weight_read_iterations);
    Weight_Loader(weight_mem1, weight_streams[1], weight_offset, weight_read_iterations);
    Weight_Loader(weight_mem2, weight_streams[2], weight_offset, weight_read_iterations);
    Weight_Loader(weight_mem3, weight_streams[3], weight_offset, weight_read_iterations);

    // load scales and distribute to scale streams
    Scale_Loader_Distributor(scale_mem, scale_dispacher, scale_offset, scale_read_iterations);

    // copy input buffer to dispacher streams
    Stream_Copy(input_buffer, dispacher, rows / NUM_LANES, cols / INP_ENTRIES);

    // perform mul-adder tree for each lane
    for(int lane = 0; lane < NUM_LANES; lane++){
        #pragma HLS UNROLL
        Mul_Adder_Tree_128(dispacher[lane], weight_streams[lane], adder_tree_output[lane], weight_read_iterations);

        // dequantize and add bias for each lane
        Accumulator_Quantizer(adder_tree_output[lane], scale_dispacher[lane], dequantized_output[lane],
                              rows / NUM_LANES, cols / INP_ENTRIES);
    }
    // collect outputs from all lanes and write to out_stream
    Bias_Merger(dequantized_output, out_stream, bias_buffer, bias_offset, rows / NUM_LANES);

}