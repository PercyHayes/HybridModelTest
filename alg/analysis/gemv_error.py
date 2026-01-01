import os
import numpy as np
from matplotlib import pyplot as plt

def load_binary_data(file_path, dtype, shape):
    """加载二进制数据并重塑为指定形状"""
    data = np.fromfile(file_path, dtype=dtype)
    return data.reshape(shape)

def analyze_gemv_error(ref_file, hls_file, out_channels=3584):
    ref_data = load_binary_data(ref_file, dtype=np.float32, shape=(out_channels,))
    hls_data = load_binary_data(hls_file, dtype=np.float32, shape=(out_channels,))
    errors = hls_data - ref_data
    print("first 10 reference data:", ref_data[:10])
    print("first 10 HLS output data:", hls_data[:10])
    abs_errors = np.abs(errors)
    rel_errors = np.abs(errors) / (np.abs(ref_data) + 1e-6)  # 避免除以零
    # 计算误差统计信息
    max_abs_error = np.max(abs_errors)
    mean_abs_error = np.mean(abs_errors)
    max_rel_error = np.max(rel_errors)
    mean_rel_error = np.mean(rel_errors)
    print(f"max absolute error: {max_abs_error:.6f}")
    print(f"mean absolute error: {mean_abs_error:.6f}")
    print(f"max relative error: {max_rel_error:.6f}")
    print(f"mean relative error: {mean_rel_error:.6f}")

    # 绘制误差分布图
    plt.figure(figsize=(12, 5))
    plt.scatter(ref_data, rel_errors, alpha=0.5,s=10,c='b',edgecolors='none')
    plt.title(f'Relative Error vs Reference Value (N={len(ref_data)})')
    plt.xlabel('Reference Value (Float32)')
    plt.ylabel('Relative Error')
    plt.grid(True, linestyle='--', alpha=0.5)

    plt.savefig('./gemv_rel_error_distribution.png', dpi=300)

if __name__ == "__main__":
    ref_file = "/home/percy/data/HDL/HybridModelTest/src/testbench/Gemv_Test/dump_ref_data.bin"
    hls_file = "/home/percy/data/HDL/HybridModelTest/src/testbench/Gemv_Test/dump_hls_output_data.bin"
    analyze_gemv_error(ref_file, hls_file, out_channels=3584)

    


