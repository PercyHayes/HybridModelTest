#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "./../../include/Gemv_Test.h"
#include "./../fp16_fix.h" 

const std::string parent_dir = "/home/percy/data/HDL/HybridModelTest/alg/gemv_3584/";
const std::string input_file = parent_dir + "gemv_input_data.bin";
const std::string bias_file = parent_dir + "gemv_bias_data.bin";
const std::string scaling_file = parent_dir + "gemv_scaling_data.bin";
const std::string weight_file_part0 = parent_dir + "gemv_weights_part0.bin";
const std::string weight_file_part1 = parent_dir + "gemv_weights_part1.bin";
const std::string weight_file_part2 = parent_dir + "gemv_weights_part2.bin";
const std::string weight_file_part3 = parent_dir + "gemv_weights_part3.bin";
const std::string output_file = parent_dir + "gemv_output_data.bin";

const int in_channels = 3584;
const int out_channels = 3584;

const int weights_per_file = out_channels / NUM_LANES * in_channels / INP_ENTRIES; // 896 * 28 = 25088
packed_io_int4 weights_part0[weights_per_file];
packed_io_int4 weights_part1[weights_per_file];
packed_io_int4 weights_part2[weights_per_file];
packed_io_int4 weights_part3[weights_per_file];

activation_t input_data[in_channels]; // 3584 fp16
activation_t bias_data[out_channels]; // 3584 fp16
activation_t ref_output_data[out_channels]; // 3584 fp16

const int scale_groups = out_channels * in_channels / (INP_ENTRIES * SCALING_INP); // 3584*3584/(128*16)=6272
packed_io_scale scaling_data[scale_groups]; // 6272 * 512 bits


void load_weights_from_file() {
    std::ifstream weight_fs0(weight_file_part0, std::ios::binary);
    std::ifstream weight_fs1(weight_file_part1, std::ios::binary);
    std::ifstream weight_fs2(weight_file_part2, std::ios::binary);
    std::ifstream weight_fs3(weight_file_part3, std::ios::binary);

    if (!weight_fs0.is_open() || !weight_fs1.is_open() || !weight_fs2.is_open() || !weight_fs3.is_open()) {
        std::cerr << "Error opening weight files!" << std::endl;
        exit(1);
    }

    // 定义预期大小
    size_t expected_size = sizeof(packed_io_int4) * weights_per_file;

    // 读取并检查
    weight_fs0.read(reinterpret_cast<char*>(weights_part0), expected_size);
    if (weight_fs0.gcount() != expected_size) {
        std::cerr << "Error: File 0 size mismatch! Expected " << expected_size << " bytes." << std::endl;
        exit(1);
    }
    
    weight_fs1.read(reinterpret_cast<char*>(weights_part1), expected_size);
    if (weight_fs1.gcount() != expected_size) {
        std::cerr << "Error: File 1 size mismatch! Expected " << expected_size << " bytes." << std::endl;
        exit(1);
    }
    
    weight_fs2.read(reinterpret_cast<char*>(weights_part2), expected_size);
    if (weight_fs2.gcount() != expected_size) {
        std::cerr << "Error: File 2 size mismatch! Expected " << expected_size << " bytes." << std::endl;
        exit(1);
    }
    
    weight_fs3.read(reinterpret_cast<char*>(weights_part3), expected_size);
    if (weight_fs3.gcount() != expected_size) {
        std::cerr << "Error: File 3 size mismatch! Expected " << expected_size << " bytes." << std::endl;
        exit(1);
    }

    weight_fs0.close();
    weight_fs1.close();
    weight_fs2.close();
    weight_fs3.close();
}

void load_fp16_data_from_file(std::string filename, activation_t* data_array, size_t expected_count) {
    std::ifstream fs(filename, std::ios::binary);
    if (!fs.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
    size_t expected_size = sizeof(activation_t) * expected_count;
    fs.read(reinterpret_cast<char*>(data_array), expected_size);
    if (fs.gcount() != expected_size) {
        std::cerr << "Error: File size mismatch for " << filename << "! Expected " << expected_size << " bytes." << std::endl;
        exit(1);    
    }
    fs.close();
}

void load_scaling_data_from_file() {
    std::ifstream scale_fs(scaling_file, std::ios::binary);
    if (!scale_fs.is_open()) {
        std::cerr << "Error opening scaling file!" << std::endl;
        exit(1);
    }
    size_t expected_size = sizeof(packed_io_scale) * scale_groups;
    scale_fs.read(reinterpret_cast<char*>(scaling_data), expected_size);
    if (scale_fs.gcount() != expected_size) {
        std::cerr << "Error: Scaling file size mismatch! Expected " << expected_size << " bytes." << std::endl;
        exit(1);
    }
    scale_fs.close();
}


int main(){
    std::cout << "--- Starting Testbench for Gemv_Test ---" << std::endl;

    // --- 1. Load all data files ---
    std::cout << "Loading weight data..." << std::endl;
    load_weights_from_file();
    std::cout << "Loading input activation data..." << std::endl;
    load_fp16_data_from_file(input_file, input_data, in_channels);
    std::cout << "Loading bias data..." << std::endl;
    load_fp16_data_from_file(bias_file, bias_data, out_channels);
    std::cout << "Loading reference output data..." << std::endl;
    load_fp16_data_from_file(output_file, ref_output_data, out_channels);
    std::cout << "Loading scaling data..." << std::endl;
    load_scaling_data_from_file();

    // print input data for debug
    /*for(int i = 0; i < in_channels; ++i){
        std::cout << "Input data[" << i << "]: " << input_data[i] << std::endl;
    }*/
    // --- 2. Create output stream and call HLS function ---
    hls::stream<packed_io_activation> out_stream("out_stream");

    //output buffer is unused in this testbench
    activation_t output_buffer[out_channels];
    
    std::cout << "\nCalling HLS function Gemv_Test..." << std::endl;
    Gemv_Test(
        weights_part0,
        weights_part1,
        weights_part2,
        weights_part3,
        scaling_data,
        out_stream,
        input_data,
        bias_data,
        output_buffer, // output_buffer unused
        out_channels,
        in_channels,
        0, // weight_offset
        0, // scale_offset
        0  // bias_offset
    );
    std::cout << "HLS function execution finished." << std::endl;

    // --- 3. Verify output against reference data ---
    std::cout << "\nVerifying output data..." << std::endl;
    bool all_passed = true;
    int error_count = 0;

    // debug files
    std::ofstream dump_ref("/home/percy/data/HDL/HybridModelTest/src/testbench/Gemv_Test/dump_ref_data.bin", std::ios::binary);
    std::ofstream dump_out("/home/percy/data/HDL/HybridModelTest/src/testbench/Gemv_Test/dump_hls_output_data.bin", std::ios::binary);
    for (int oc = 0; oc < out_channels / NUM_LANES; ++oc) { // every NUM_LANES outputs packed in one packed_io_activation
        packed_io_activation out_data_packed = out_stream.read();
        for(int lane = 0; lane < NUM_LANES; ++lane){
            int out_ch = oc * NUM_LANES + lane;
            // Extract the relevant activation_t from packed_io_activation
            ap_uint<ACTIVATION_BITS> raw_bits = out_data_packed.range((lane + 1) * ACTIVATION_BITS - 1, lane * ACTIVATION_BITS);
            activation_t out_data;
            //out_data = static_cast<activation_t>(raw_bits);
            out_data = *reinterpret_cast<activation_t*>(&raw_bits);
            /*if(out_ch <10){
                std::cout << "Output data channel " << out_ch << ": " << out_data << std::endl;
            }*/
            
            activation_t ref_data = ref_output_data[out_ch];
            float out_data_float = static_cast<float>(out_data);
            float ref_data_float = static_cast<float>(ref_data);
            //// save data for analysis
            dump_ref.write(reinterpret_cast<const char*>(&ref_data_float), sizeof(float));
            dump_out.write(reinterpret_cast<const char*>(&out_data_float), sizeof(float));
            // Use a tolerance for floating-point comparison
            float tolerance = 0.02f;
            float abs_error = std::fabs(out_data_float - ref_data_float);
            if (abs_error > tolerance && abs_error / std::fabs(ref_data_float) > tolerance) {
                error_count++;
                std::cout << std::fixed << std::setprecision(6);
                std::cout << "Mismatch at output channel " << out_ch
                          << ": expected " << ref_data_float
                          << ", got " << out_data_float
                          << ", abs error " << abs_error << std::endl;
            }
        }
    }
    if (error_count == 0) {
        all_passed = true;
        std::cout << "All outputs match the reference data. Test PASSED." << std::endl;
    } else {
        all_passed = false;
        std::cout << error_count << " mismatches found. Test FAILED." << std::endl;
    }
    std::cout << "--- Testbench Finished ---" << std::endl;
    return all_passed ? 0 : 1;
}
