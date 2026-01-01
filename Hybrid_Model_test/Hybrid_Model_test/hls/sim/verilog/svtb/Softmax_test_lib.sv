//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef SOFTMAX_TEST_LIB__SV                                    
    `define SOFTMAX_TEST_LIB__SV                                
                                                                          
    `include "uvm_macros.svh"                                           
                                                                          
    class Softmax_test_lib extends uvm_test;                          
                                                                          
        Softmax_env top_env;                                          
                                                                          
        `uvm_component_utils(Softmax_test_lib)                        
                                                                          
        function new (string name, uvm_component parent = null);          
            super.new(name, parent);                                      
        endfunction                                                       
                                                                          
        virtual function void build_phase(uvm_phase phase);               
            super.build_phase(phase);                                     
            top_env = Softmax_subsystem_pkg::Softmax_env::type_id::create("top_env", this);
            uvm_config_db#(uvm_object_wrapper)::set(                      
                    this,                                                 
                    "top_env.Softmax_virtual_sqr.run_phase",        
                    "default_sequence",                                 
                    Softmax_subsys_test_sequence_lib::type_id::get()  
                );                                                        
            `uvm_info(this.get_full_name(), "build_phase done", UVM_LOW)
        endfunction                                                       
        virtual function void end_of_elaboration_phase(uvm_phase phase);  
            uvm_root top = uvm_root::get();                               
            top.print_topology();                                         
        endfunction                                                       
    endclass                                                              
                                                                          
`endif                                                                    
