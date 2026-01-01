#include "./../include/SoftMax.h"

// 80% active cycles 32764/36945

void Softmax( 
    hls::stream<activation_t>& in_stream,
    hls::stream<activation_t>& out_stream,
    const int seq_len
){
    /**** interface for co-simulation****/
    #pragma HLS INTERFACE axis port=in_stream depth=16384
    #pragma HLS INTERFACE axis port=out_stream depth=16384
    #pragma HLS INTERFACE s_axilite port=seq_len bundle=CTRL_BUS
    /**************************************/

    #pragma HLS DATAFLOW

    hls::stream<activation_t> max_value_stream;
    #pragma HLS STREAM variable=max_value_stream depth=4  // adjust depth
    #pragma HLS BIND_STORAGE variable = max_value_stream type = fifo impl = srl

    hls::stream<activation_t> sum_exp_stream;
    #pragma HLS STREAM variable=sum_exp_stream depth=4  // adjust depth
    #pragma HLS BIND_STORAGE variable = sum_exp_stream type = fifo impl = srl

    hls::stream<activation_t> inv_sum_exp_stream;
    #pragma HLS STREAM variable=inv_sum_exp_stream depth=4  // adjust depth
    #pragma HLS BIND_STORAGE variable = inv_sum_exp_stream type = fifo impl = srl

    hls::stream<activation_t> max_bypass_stream;
    #pragma HLS STREAM variable=max_bypass_stream depth=16384  // adjust depth
    #pragma HLS BIND_STORAGE variable = max_bypass_stream type = fifo impl = uram

    hls::stream<activation_t> exp_bypass_stream;
    #pragma HLS STREAM variable=exp_bypass_stream depth=16384  // adjust depth
    #pragma HLS BIND_STORAGE variable = exp_bypass_stream type = fifo impl = uram

    // find max value
    max_value:
    for(int head = 0; head < Q_HEAD_NUM / HEAD_PARALLELISM; head++){
        #pragma HLS LOOP_TRIPCOUNT min=4 max=4
        activation_t max_val = -65504.0; // minimum half value
        for(int i = 0; i < seq_len; i++){
            #pragma HLS LOOP_TRIPCOUNT min=4096 max=4096
            #pragma HLS PIPELINE II=1
            activation_t val = in_stream.read();
            max_val = hls::max(max_val, val);
            max_bypass_stream.write(val);
        }
        max_value_stream.write(max_val);
    }

    sub_max:
    for(int head = 0; head < Q_HEAD_NUM / HEAD_PARALLELISM; head++){
        #pragma HLS LOOP_TRIPCOUNT min=4 max=4
        activation_t max_val = max_value_stream.read();
        //half sum_exp = 0.0;
        acc_t sum_exp_acc = 0.0;
        for(int i = 0; i < seq_len; i++){
            #pragma HLS LOOP_TRIPCOUNT min=4096 max=4096
            #pragma HLS PIPELINE II=1
            const half small_value = 1e-6;
            activation_t val = max_bypass_stream.read();
            activation_t sub_val = val - max_val;
            // compute exp
            activation_t exp_val = hls::half_exp(sub_val) + small_value;

            // sum
            sum_exp_acc += acc_t(exp_val);
            exp_bypass_stream.write(exp_val);
        }
        sum_exp_stream.write(sum_exp_acc.to_half());
    }


    divide:
    for(int head = 0; head < Q_HEAD_NUM / HEAD_PARALLELISM; head++){
        #pragma HLS LOOP_TRIPCOUNT min=4 max=4
        activation_t sum_exp = sum_exp_stream.read();
        activation_t inv_sum_exp = 1.0 / sum_exp;
        inv_sum_exp_stream.write(inv_sum_exp);
    }

    softmax_output:
    for(int head = 0; head < Q_HEAD_NUM / HEAD_PARALLELISM; head++){
        #pragma HLS LOOP_TRIPCOUNT min=4 max=4
        activation_t inv_sum_exp = inv_sum_exp_stream.read();
        for(int i = 0; i < seq_len; i++){
            #pragma HLS LOOP_TRIPCOUNT min=4096 max=4096
            #pragma HLS PIPELINE II=1
            activation_t exp_val = exp_bypass_stream.read();
            // final softmax output
            activation_t softmax_val = exp_val * inv_sum_exp;
            out_stream.write(softmax_val);
        }
    }
}