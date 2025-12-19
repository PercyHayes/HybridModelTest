import os
import numpy as np

def generate_mul128_data(iterations=3584, input_size=128, weight_size=128):
    """
    生成测试数据，包括输入、权重和输出
    
    参数:
        iterations: 生成的数据组数
        input_size: 输入数据的维度
        weight_size: 权重的维度
    """
    
    # 创建输出目录
    output_dir = os.path.dirname(os.path.abspath(__file__))
    os.makedirs(output_dir, exist_ok=True)
    
    print(f"生成 {iterations} 组测试数据...")
    print(f"输入数据维度: {input_size} (fp16)")
    print(f"权重维度: {weight_size} (int4)")
    print(f"输出维度: 1 (fp16) - 乘法累加树结果")
    
    # 生成随机输入数据 (fp16)
    # 范围在 -1.0 到 1.0 之间
    input_data = np.random.uniform(-2.0, 2.0, (iterations, input_size)).astype(np.float16)
    
    # 生成随机权重 (int4: -8 到 7)
    # int4的有效范围是 -8 到 7
    weights = np.random.randint(-8, 8, (iterations, weight_size), dtype=np.int8)
    
    # 计算输出 (fp16)
    # 乘法累加树：先逐元素相乘，然后累加成一个值（点积运算）
    output_data = np.zeros(iterations, dtype=np.float16)
    
    for i in range(iterations):
        # 1. 逐元素相乘：128个输入 * 128个权重
        mult_result = input_data[i] * weights[i].astype(np.float16)
        # 2. 累加所有乘积结果（模拟加法树：128->64->32->16->8->4->2->1）
        output_data[i] = np.sum(mult_result).astype(np.float16)
    
    # 将权重打包为int4格式（2个int4打包成1个字节）
    # 128个int4权重 -> 64个字节
    packed_weights = np.zeros((iterations, weight_size // 2), dtype=np.uint8)
    for i in range(iterations):
        for j in range(0, weight_size, 2):
            # 将两个int4值打包到一个字节中
            # 低4位存第一个int4，高4位存第二个int4
            val0 = weights[i, j] & 0x0F  # 取低4位
            val1 = weights[i, j+1] & 0x0F  # 取低4位
            packed_weights[i, j//2] = (val1 << 4) | val0
    
    # 保存为二进制文件
    input_file = os.path.join(output_dir, 'input_data.bin')
    weight_file = os.path.join(output_dir, 'weight_data.bin')
    output_file = os.path.join(output_dir, 'output_data.bin')
    
    # 保存输入数据 (fp16)
    input_data.tofile(input_file)
    print(f"✓ 输入数据已保存: {input_file}")
    print(f"  大小: {os.path.getsize(input_file)} bytes")
    
    # 保存权重数据 (严格int4格式：2个int4打包成1个字节)
    packed_weights.tofile(weight_file)
    print(f"✓ 权重数据已保存: {weight_file}")
    print(f"  大小: {os.path.getsize(weight_file)} bytes (每个权重占4位，128个权重=64字节)")
    print(f"  原始权重数: {iterations * weight_size}, 打包后字节数: {iterations * weight_size // 2}")
    
    # 保存输出数据 (fp16)
    output_data.tofile(output_file)
    print(f"✓ 输出数据已保存: {output_file}")
    print(f"  大小: {os.path.getsize(output_file)} bytes")
    
    # 保存一些统计信息
    print("\n数据统计信息:")
    print(f"输入数据范围: [{input_data.min():.4f}, {input_data.max():.4f}]")
    print(f"权重范围: [{weights.min()}, {weights.max()}]")
    print(f"输出数据范围: [{output_data.min():.4f}, {output_data.max():.4f}]")
    
    # 验证第一组数据
    print("\n第一组数据验证:")
    print(f"  输入前5个元素: {input_data[0, :5]}")
    print(f"  权重前5个元素: {weights[0, :5]}")
    print(f"  输出结果: {output_data[0]:.6f}")
    print(f"  (验证计算: {np.sum(input_data[0] * weights[0].astype(np.float16)):.6f})")
    
    print(f"first 10 results: {output_data[:10]}")
    return input_data, weights, output_data


if __name__ == "__main__":
    # 生成测试数据
    generate_mul128_data(iterations=3584, input_size=128, weight_size=128)
    print("\n✓ 测试数据生成完成！")
    