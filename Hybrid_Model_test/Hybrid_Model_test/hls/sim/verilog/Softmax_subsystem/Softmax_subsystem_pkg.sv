//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`timescale 1ns/1ps 

`ifndef SOFTMAX_SUBSYSTEM_PKG__SV          
    `define SOFTMAX_SUBSYSTEM_PKG__SV      
                                                     
    package Softmax_subsystem_pkg;               
                                                     
        import uvm_pkg::*;                           
        import file_agent_pkg::*;                    
        import svr_pkg::*;
        import axi_pkg::*;
                                                     
        `include "uvm_macros.svh"                  
                                                     
        `include "Softmax_config.sv"           
        `include "Softmax_reference_model.sv"  
        `include "Softmax_scoreboard.sv"       
        `include "Softmax_subsystem_monitor.sv"
        `include "Softmax_virtual_sequencer.sv"
        `include "Softmax_pkg_sequence_lib.sv" 
        `include "Softmax_env.sv"              
                                                     
    endpackage                                       
                                                     
`endif                                               
