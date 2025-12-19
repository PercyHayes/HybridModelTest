#ifndef FP16_FIX_H
#define FP16_FIX_H

#include "hls_half.h" 

// ------------------------------------------------------------------
// Vitis HLS Co-Sim Linker Fix
// 解决 Vitis 自带 libgcc_s.so 缺失半精度浮点转换符号的问题
// ------------------------------------------------------------------
extern "C" {
    // 必须使用 extern "C"，因为链接器寻找的是未修饰的 C 符号名

    // 半精度 (16-bit) -> 单精度 (32-bit)
    float __gnu_h2f_ieee(unsigned short h) {
        half h_val; // 去掉 hls::
        h_val.set_bits(h);
        return (float)h_val;
    }

    // 单精度 (32-bit) -> 半精度 (16-bit)
    unsigned short __gnu_f2h_ieee(float f) {
        half h_val = (half)f; // 去掉 hls::
        return h_val.get_bits();
    }
}

#endif // FP16_FIX_H