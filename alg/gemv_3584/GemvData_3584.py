import numpy as np

def generate_vector_data(vector_size=3584):
    '''
    生成测试输入向量，fp16格式，范围在-2.0到2.0之间
    '''
    # 生成随机输入数据 (fp16)
    input_data = np.random.uniform(-2.0, 2.0, (vector_size,)).astype(np.float16)
    return input_data

def generate_int4_weights(rows=3584, cols=3584):
    '''
    生成测试权重，int4格式，范围在-8到7之间
    '''
    # 生成随机权重 (int4: -8 到 7)
    weights = np.random.uniform(-8, 8, (rows, cols)).clip(-8, 7).round().astype(np.int8)
    #randint(-8, 8, (rows, cols), dtype=np.int8)
    return weights

def generate_scaling_data(rows=3584, cols=3584, groups=128):
    '''
    生成缩放因子数据，fp16格式，范围在-1.0到1.0之间
    '''
    scaling_cols = cols // groups
    scaling_data = np.random.uniform(-0.05, 0.05, (rows, scaling_cols)).astype(np.float16)
    return scaling_data

def generate_bias_data(rows=3584):
    '''
    生成偏置数据，fp16格式，范围在-1.0到1.0之间
    '''
    bias_data = np.random.uniform(-1.0, 1.0, (rows,)).astype(np.float16)
    return bias_data

def generate_gemv_results(input_data, weights, scaling_data, bias_data, groups=128):
    '''
    计算Gemv的输出结果，fp16格式
    '''
    expanded_scaling = np.repeat(scaling_data, groups, axis=1)
    #print("expanded_scaling shape:", expanded_scaling[0][0:16])
    weights_fp16 = weights.astype(np.float16) * expanded_scaling
    dot_product = np.dot(weights_fp16, input_data)
    #print("dot_product shape:", dot_product.shape)
    final_results = dot_product + bias_data
    return final_results.astype(np.float16)

def save_weight_files(weights, num_spilts = 4, filename='./weight_data'):
    '''
    将int4格式的权重重排列并打包成二进制文件
    '''
    rows, cols = weights.shape
    assert cols % 2 == 0, "cols must be even for int4 packing"
    assert rows % num_spilts == 0, "rows must be divisible by num_splits"

    for i in range(num_spilts):
        spilt_weights = weights[i::num_spilts, :]
        low_nibble = spilt_weights[:, 0::2] & 0x0F
        high_nibble = spilt_weights[:, 1::2] & 0x0F
        packed_data = (low_nibble | (high_nibble << 4)).astype(np.uint8)
        if i == 0:
            print("first 16 packed weights (binary):", ['{0:08b}'.format(b) for b in packed_data[0][0:16]])
        file_name_full = f"{filename}_part{i}.bin"
        packed_data.tofile(file_name_full)

def save_scaling_file(scaling_data, num_lanes=4, filename='./scaling_data.bin'):
    '''
    保存缩放因子数据为二进制文件
    '''
    rows, cols = scaling_data.shape
    assert rows % num_lanes == 0, "rows must be divisible by num_lanes"

    fixed_scaling = (scaling_data.astype(np.float32) * 65536.0).round().astype(np.int32)
    #reorder for lane processing
    reshaped_scaling = fixed_scaling.reshape(rows // num_lanes, num_lanes, cols)
    transposed_scaling = reshaped_scaling.transpose(0,2,1) #896,28,4
    transposed_scaling.tofile(filename)
    
def save_vector_file(vector_data, filename='./bias_data.bin'):
    '''
    保存偏置数据为二进制文件
    '''
    vector_data.tofile(filename)


if __name__ == "__main__":
    rows = 3584
    cols = 3584
    groups = 128

    input_data = generate_vector_data(cols)
    weights = generate_int4_weights(rows, cols)
    
    scaling_data = generate_scaling_data(rows, cols, groups)
    bias_data = generate_bias_data(rows)
    output_data = generate_gemv_results(input_data, weights, scaling_data, bias_data, groups)
    #print("output_data shape:", output_data.shape)

    #data for verification
    print("------------------------------------------------------------------------")
    print("first 10 input data:", input_data[0:10])
    print("first 10 weights row 0:", weights[0][0:10])
    print("first 10 scaling data row 0:", scaling_data[0][0:10])
    print("first 10 bias data:", bias_data[0:10])
    print("first 10 output data:", output_data[0:10])
    print("------------------------------------------------------------------------")

    save_weight_files(weights, num_spilts=4, filename='./gemv_weights')
    save_scaling_file(scaling_data, num_lanes=4, filename='./gemv_scaling_data.bin')
    save_vector_file(bias_data, filename='./gemv_bias_data.bin')
    save_vector_file(input_data, filename='./gemv_input_data.bin')
    save_vector_file(output_data, filename='./gemv_output_data.bin')
    

   