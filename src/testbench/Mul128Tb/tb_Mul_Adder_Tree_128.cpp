#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "./../../include/Mul_Adder_Tree_128.h"
#include "./../fp16_fix.h" 
// ------------------------------------------------------------------
/*extern "C" {
    // 半精度 (16-bit) 转 单精度 (32-bit)
    float __gnu_h2f_ieee(unsigned short h) {
        half h_val;
        h_val.set_bits(h);
        return (float)h_val;
    }

    // 单精度 (32-bit) 转 半精度 (16-bit)
    unsigned short __gnu_f2h_ieee(float f) {
        half h_val = (half)f;
        return h_val.get_bits();
    }
}*/
// ------------------------------------------------------------------

// ...existing code...

const int ITERATIONS = 3584;
const std::string parent_dir = "/home/percy/data/HDL/HybridModelTest/alg/Mul128Data/";
const std::string input_file = parent_dir + "input_data.bin";
const std::string weight_file = parent_dir + "weight_data.bin";
const std::string output_file = parent_dir + "output_data.bin";

// Global arrays - use float for host-side to avoid fp16 conversion issues
packed_io_int4 weights[ITERATIONS];
float input_data_float[ITERATIONS][INP_ENTRIES];  // Store as float on host
float ref_output_data_float[ITERATIONS];           // Store as float on host

// Helper function to convert fp16 (as uint16) to float manually to avoid linker errors
float fp16_to_float(uint16_t h) {
    uint16_t sign = (h >> 15) & 0x1;
    uint16_t exp = (h >> 10) & 0x1F;
    uint16_t mant = h & 0x3FF;
    
    if (exp == 0) {
        if (mant == 0) return sign ? -0.0f : 0.0f;
        // Subnormal
        return (sign ? -1.0f : 1.0f) * std::ldexp((float)mant, -24);
    } else if (exp == 31) {
        if (mant == 0) return sign ? -INFINITY : INFINITY;
        return NAN;
    }
    // Normal
    return (sign ? -1.0f : 1.0f) * std::ldexp((float)(mant + 1024), exp - 25);
}

// Load weight data (packed int4 format: 128 int4 = 64 bytes = 512 bits)
void load_weights(){
    std::cout << "Loading packed weight data from '" << weight_file << "'..." << std::endl;
    std::ifstream weight_in(weight_file, std::ios::binary);
    if(!weight_in.is_open()){
        std::cerr << "ERROR: Could not open weight file: " << weight_file << std::endl;
        exit(1);
    }
    
    const int BYTES_PER_WORD = 64; // 512 bits / 8 = 64 bytes
    char buffer[BYTES_PER_WORD];
    
    for(int i = 0; i < ITERATIONS; ++i){
        if(!weight_in.read(buffer, BYTES_PER_WORD)){
            std::cerr << "ERROR: File ended prematurely at iteration " << i << std::endl;
            exit(1);
        }
        
        packed_io_int4 packed_word = 0;
        // Reconstruct the 512-bit word from bytes
        for(int b = 0; b < BYTES_PER_WORD; ++b){
            packed_word.range((b + 1) * 8 - 1, b * 8) = (unsigned char)buffer[b];
        }
        weights[i] = packed_word;
    }
    
    weight_in.close();
    std::cout << "Successfully loaded " << ITERATIONS << " packed weight vectors." << std::endl;
}

// Load input data (fp16 format: 128 half values per iteration)
void load_inputs(){
    std::cout << "Loading input data from '" << input_file << "'..." << std::endl;
    std::ifstream input_in(input_file, std::ios::binary);
    if(!input_in.is_open()){
        std::cerr << "ERROR: Could not open input file: " << input_file << std::endl;
        exit(1);
    }
    
    const int BYTES_PER_HALF = 2; // sizeof(half) = 2 bytes
    char buffer[BYTES_PER_HALF];
    
    // Read as raw bytes and convert to float on host
    for(int i = 0; i < ITERATIONS; ++i){
        for(int j = 0; j < INP_ENTRIES; ++j){
            if(!input_in.read(buffer, BYTES_PER_HALF)){
                std::cerr << "ERROR: File ended prematurely at iteration " << i << ", entry " << j << std::endl;
                exit(1);
            }
            // Reconstruct half from bytes and convert to float
            uint16_t half_bits = 0;
            for(int b = 0; b < BYTES_PER_HALF; ++b){
                // Little-endian reconstruction
                half_bits |= ((uint8_t)buffer[b] << (b * 8));
            }
            input_data_float[i][j] = fp16_to_float(half_bits);
        }
    }
    
    input_in.close();
    std::cout << "Successfully loaded " << ITERATIONS << " input vectors." << std::endl;
}

// Load reference output data (fp16 format: 1 half value per iteration)
void load_reference_outputs(){
    std::cout << "Loading reference output data from '" << output_file << "'..." << std::endl;
    std::ifstream output_in(output_file, std::ios::binary);
    if(!output_in.is_open()){
        std::cerr << "ERROR: Could not open output file: " << output_file << std::endl;
        exit(1);
    }
    
    const int BYTES_PER_HALF = 2;
    char buffer[BYTES_PER_HALF];
    
    // Read as raw bytes and convert to float on host
    for(int i = 0; i < ITERATIONS; ++i){
        if(!output_in.read(buffer, BYTES_PER_HALF)){
            std::cerr << "ERROR: Failed to read reference output at iteration " << i << std::endl;
            exit(1);
        }
        // Reconstruct half from bytes and convert to float
        uint16_t half_bits = 0;
        for(int b = 0; b < BYTES_PER_HALF; ++b){
            // Little-endian reconstruction
            half_bits |= ((uint8_t)buffer[b] << (b * 8));
        }
        ref_output_data_float[i] = fp16_to_float(half_bits);
    }
    
    output_in.close();
    std::cout << "Successfully loaded " << ITERATIONS << " reference outputs." << std::endl;
}

int main(){
    std::cout << "--- Starting Testbench for Mul_Adder_Tree_128 ---" << std::endl;
    
    // --- 1. Load all data files ---
    load_inputs();
    load_weights();
    load_reference_outputs();
    
    // --- 2. Create streams and feed data to HLS function ---
    std::cout << "\nCalling HLS function Mul_Adder_Tree_128..." << std::endl;
    hls::stream<activation_vector_128> data_stream("data_stream");
    hls::stream<packed_io_int4> weight_stream("weight_stream");
    hls::stream<acc_t> result_stream("result_stream");
    
    // Feed input data to streams (convert float to half at DUT interface)
    for(int i = 0; i < ITERATIONS; ++i){
        activation_vector_128 input_vec;
        for(int j = 0; j < INP_ENTRIES; ++j){
            input_vec[j] = half(input_data_float[i][j]);
        }
        data_stream.write(input_vec);
        weight_stream.write(weights[i]);
    }
    
    // Call the DUT (Device Under Test)
    Mul_Adder_Tree_128(data_stream, weight_stream, result_stream, ITERATIONS);
    
    std::cout << "HLS function execution finished." << std::endl;
    
    // --- 3. Verify output against reference data ---
    std::cout << "\nVerifying output against reference data..." << std::endl;
    int errors = 0;
    double max_error = 0.0;
    double total_error = 0.0;
    
    for(int i = 0; i < ITERATIONS; ++i){
        activation_t dut_output = result_stream.read();
        float ref_value = ref_output_data_float[i];
        
        float dut_value = static_cast<float>(dut_output);
        double abs_error = std::abs(static_cast<double>(dut_value) - static_cast<double>(ref_value));
        
        // Calculate relative error for better assessment
        double rel_error = 0.0;
        if(std::abs(ref_value) > 1e-6){
            rel_error = abs_error / std::abs(ref_value);
        }
        
        total_error += abs_error;
        if(abs_error > max_error){
            max_error = abs_error;
        }
        
        // Tolerance for half precision (fp16)
        if(rel_error > 0.01 && abs_error > 0.4){ // 1% relative or 0.1 absolute
            if(errors < 10){ // Print first 10 errors
                std::cout << std::fixed << std::setprecision(6)
                          << "Mismatch at iteration " << i 
                          << ": DUT=" << dut_value 
                          << ", REF=" << ref_value 
                          << ", AbsError=" << abs_error 
                          << ", RelError=" << (rel_error * 100.0) << "%" << std::endl;
            }
            errors++;
        }
    }
    
    // --- 4. Print summary ---
    std::cout << "\n========== Test Summary ==========" << std::endl;
    std::cout << "Total iterations: " << ITERATIONS << std::endl;
    
    if(errors == 0){
        std::cout << "✓ All outputs match the reference data!" << std::endl;
    } else {
        std::cout << "✗ Total mismatches: " << errors << " out of " << ITERATIONS << " outputs." << std::endl;
        std::cout << "  Mismatch rate: " << std::fixed << std::setprecision(2) 
                  << (100.0 * errors / ITERATIONS) << "%" << std::endl;
    }
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Maximum absolute error: " << max_error << std::endl;
    std::cout << "Average absolute error: " << (total_error / ITERATIONS) << std::endl;
    std::cout << "==================================\n" << std::endl;
    
    std::cout << "--- Testbench Finished ---" << std::endl;
    return (errors == 0) ? 0 : 1;
}
