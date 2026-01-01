#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "./../../include/SoftMax.h"
#include "./../fp16_fix.h" 

const int SEQ_LEN = 4096;
const int HEADS = 4;
const std::string parent_dir = "/home/percy/data/HDL/HybridModelTest/alg/softmax/";
const std::string input_file = parent_dir + "softmax_input.bin";
const std::string output_file = parent_dir + "softmax_output.bin";

activation_t input_data[HEADS*SEQ_LEN];
activation_t ref_output_data[HEADS*SEQ_LEN];

// Load input data (fp16 format: 1 half value per entry)
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

int main(){
    std::cout << "--- Starting Testbench for Softmax ---" << std::endl;

    // --- 1. Load all data files ---
    std::cout << "Loading input activation data..." << std::endl;
    load_fp16_data_from_file(input_file, input_data, HEADS * SEQ_LEN);
    std::cout << "Loading reference output data..." << std::endl;
    load_fp16_data_from_file(output_file, ref_output_data, HEADS * SEQ_LEN);

    // --- 2. Create streams and feed data to HLS function ---
    std::cout << "\nCalling HLS function Softmax..." << std::endl;
    hls::stream<activation_t> in_stream("in_stream");
    hls::stream<activation_t> out_stream("out_stream");

    // Feed input data to stream
    for(int i = 0; i < HEADS * SEQ_LEN; ++i){
        in_stream.write(input_data[i]);
    }

    // Call the DUT (Device Under Test)
    Softmax(in_stream, out_stream, SEQ_LEN);

    std::cout << "HLS function execution finished." << std::endl;

    // --- 3. Verify output against reference data ---
    std::cout << "\nVerifying output against reference data..." << std::endl;
    int errors = 0;
    double max_error = 0.0;
    double total_error = 0.0;

    for(int i = 0; i < HEADS * SEQ_LEN; ++i){
        activation_t dut_output = out_stream.read();
        activation_t ref_value = ref_output_data[i];

        float dut_value = static_cast<float>(dut_output);
        float ref_value_float = static_cast<float>(ref_value);
        double abs_error = std::abs(static_cast<double>(dut_value) - static_cast<double>(ref_value_float));

        total_error += abs_error;
        if(abs_error > max_error){
            max_error = abs_error;
        }

        // Tolerance for half precision (fp16)
        if(abs_error > 0.01){ // absolute error
            if(errors < 10){ // Print first 10 errors
                std::cout << std::fixed << std::setprecision(6)
                          << "Mismatch at index " << i
                          << ": expected " << ref_value_float
                          << ", got " << dut_value
                          << ", abs error = " << abs_error << std::endl;
            }
            errors++;
        }
    }
    double avg_error = total_error / (HEADS * SEQ_LEN);
    if(errors == 0){
        std::cout << "All outputs match reference data within tolerance." << std::endl;
    } else {
        std::cout << errors << " mismatches found!" << std::endl;
    }
    std::cout << "Max absolute error: " << max_error << std::endl;
    std::cout << "Average absolute error: " << avg_error << std::endl;
    std::cout << "--- Testbench Finished ---" << std::endl;
    return errors==0 ? 0 : 1;
}