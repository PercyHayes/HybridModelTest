import os
import numpy as np

def generate_softmax_data(seq_len=4096,heads=4):
    input_seq = np.random.uniform(-1.0, 1.0, (heads, seq_len)).astype(np.float16)
    #softmax computation
    output_seq = np.zeros((heads, seq_len), dtype=np.float16)
    for h in range(heads):
        max_val = np.max(input_seq[h])
        exp_vals = np.exp(input_seq[h] - max_val)
        sum_exp = np.sum(exp_vals)
        output_seq[h] = exp_vals / (sum_exp + 1e-6)  # Avoid division by zero

    return input_seq.astype(np.float16), output_seq.astype(np.float16)

def save_data(data, filename):
    data.tofile(filename)

if __name__ == "__main__":
    seq_len = 4096
    heads = 4
    input_data, output_data = generate_softmax_data(seq_len, heads)

    #output_dir = "softmax_data"
    #os.makedirs(output_dir, exist_ok=True)

    #input_file = os.path.join(output_dir, './softmax_input.bin')
    #output_file = os.path.join(output_dir, './softmax_output.bin')
    input_file ="./softmax_input.bin"
    output_file ="./softmax_output.bin"

    save_data(input_data, input_file)
    print(f"✓ Softmax 输入数据已保存: {input_file}")
    print(f"  大小: {os.path.getsize(input_file)} bytes")

    save_data(output_data, output_file)
    print(f"✓ Softmax 输出数据已保存: {output_file}")
    print(f"  大小: {os.path.getsize(output_file)} bytes")