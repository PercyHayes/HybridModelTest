#include "./../include/Mul_Adder_Tree_128.h"

extern "C++"{
void Mul_Adder_Tree_128(
    hls::stream<activation_vector_128>& data_stream,    // activation vector stream 128*16bits
    hls::stream<packed_io_int4>& weight_stream,       // weight vector stream 128*4bits
    hls::stream<acc_t>& result_stream,         // output activation stream 16bits
    const int Iterations
){

    /*mac: result: 3974cycles 128DSP + 30750FFs + 63873LUTs
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=3584 max=3584
        #pragma HLS PIPELINE II=1
        activation_vector_128 data_vec = data_stream.read();
        packed_io_int4 weight_vec = weight_stream.read();

        activation_t acc = 0;
        for(int i = 0; i < INP_ENTRIES; i++){
            #pragma HLS UNROLL
            int4_t weight = weight_vec.range((i+1)*QUANTIZATION_BITS-1, i*QUANTIZATION_BITS);
            activation_t data = data_vec[i];
            acc += data * activation_t(weight);
            #pragma HLS bind_op variable=acc op=mul impl=dsp
        }
        result_stream.write(acc);
    }*/

    // mul_adder_tree: result: 3619cycles 128DSP + 33611FFs + 41113LUTs
    #pragma HLS DATAFLOW

    hls::stream<activation_vector_128> pass_128;
    #pragma HLS STREAM variable=pass_128 depth=2
    #pragma HLS BIND_STORAGE variable = pass_128 type = fifo impl = srl

    hls::stream<activation_vector_64> pass_64;
    #pragma HLS STREAM variable=pass_64 depth=2
    #pragma HLS BIND_STORAGE variable = pass_64 type = fifo impl = srl

    hls::stream<activation_vector_32> pass_32;
    #pragma HLS STREAM variable=pass_32 depth=2
    #pragma HLS BIND_STORAGE variable = pass_32 type = fifo impl = srl

    hls::stream<activation_vector_16> pass_16;
    #pragma HLS STREAM variable=pass_16 depth=2
    #pragma HLS BIND_STORAGE variable = pass_16 type = fifo impl = srl

    hls::stream<activation_vector_8> pass_8;
    #pragma HLS STREAM variable=pass_8 depth=2
    #pragma HLS BIND_STORAGE variable = pass_8 type = fifo impl = srl

    hls::stream<activation_vector_4> pass_4;
    #pragma HLS STREAM variable=pass_4 depth=2
    #pragma HLS BIND_STORAGE variable = pass_4 type = fifo impl = srl

    hls::stream<activation_vector_2> pass_2;
    #pragma HLS STREAM variable=pass_2 depth=2
    #pragma HLS BIND_STORAGE variable = pass_2 type = fifo impl = srl


    // stage1 128 multiply
    mul:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_128 data_vec = data_stream.read();
        packed_io_int4 weight_vec = weight_stream.read();

        activation_vector_128 mult_vec;
        for(int i = 0; i < INP_ENTRIES; i++){
            #pragma HLS UNROLL
            int4_t weight = weight_vec.range((i+1)*QUANTIZATION_BITS-1, i*QUANTIZATION_BITS);
            activation_t data = data_vec[i];

            //for test
            #ifndef __SYNTHESIS__
            if(iter == 0 && i <= 10){
                std::cout << "data: " << data << ", weight: " << int(weight) << std::endl;
            }
            if(std::isnan(float(data * activation_t(weight)))){
                std::cout << "NaN detected in Mul_Adder_Tree_128 level 1 at iteration " << iter << ", index " << i << ", data: " << data << ", weight: " << int(weight) << std::endl;
                exit(1);
            }
            
            #endif
            mult_vec[i] = data * activation_t(weight);
            
        }
        pass_128.write(mult_vec);
    }


    // stage2 adder 128 -> 64
    adder_128_64:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_128 mult_vec = pass_128.read();
        activation_vector_64 to_pass;
        for(int i = 0; i < 64; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_64.write(to_pass);
    }

    // stage3 adder 64 -> 32
    adder_64_32:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_64 mult_vec = pass_64.read();
        activation_vector_32 to_pass;
        for(int i = 0; i < 32; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_32.write(to_pass); 
    }


    // stage4 adder 32 -> 16
    adder_32_16:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_32 mult_vec = pass_32.read();
        activation_vector_16 to_pass;
        for(int i = 0; i < 16; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_16.write(to_pass);     
    }


    // stage5 adder 16 -> 8
    adder_16_8:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_16 mult_vec = pass_16.read();
        activation_vector_8 to_pass;
        for(int i = 0; i < 8; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_8.write(to_pass);
    }

    // stage6 adder 8 -> 4
    adder_8_4:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_8 mult_vec = pass_8.read();
        activation_vector_4 to_pass;
        for(int i = 0; i < 4; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_4.write(to_pass);
    }


    // stage7 adder 4 -> 2
    adder_4_2:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_4 mult_vec = pass_4.read();
        activation_vector_2 to_pass;
        for(int i = 0; i < 2; i++){
            #pragma HLS UNROLL
            to_pass[i] = mult_vec[2*i] + mult_vec[2*i+1];
        }
        pass_2.write(to_pass);
    }

    // stage8 adder 2 -> 1
    adder_2_1:
    for(int iter = 0; iter < Iterations; iter++){
        #pragma HLS LOOP_TRIPCOUNT min=25088 max=25088
        #pragma HLS PIPELINE II=1
        activation_vector_2 mult_vec = pass_2.read();
        activation_t result = mult_vec[0] + mult_vec[1];
        #ifndef __SYNTHESIS__
            if(iter == 3584){
                std::cout << "result: " << result << std::endl;
            }
            if(std::isnan((float)result)){
                std::cout << "NaN detected in Mul_Adder_Tree_128 at iteration " << iter << std::endl;
            }
        #endif
        result_stream.write(acc_t(result));
    }

}
}