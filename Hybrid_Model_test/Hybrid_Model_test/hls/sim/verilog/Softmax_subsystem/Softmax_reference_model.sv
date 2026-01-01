//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef SOFTMAX_REFERENCE_MODEL_SV
`define SOFTMAX_REFERENCE_MODEL_SV

class Softmax_reference_model extends uvm_component;
`define TV_IN_seq_len "../tv/cdatafile/c.Softmax.autotvin_seq_len.dat"
`define TV_OUT_seq_len ""
    bit  write_data_finish_CTRL_BUS;
    event allaxilite_write_data_finish;
    event allaxilite_write_one_transaction_finish;
    event allsvr_input_done;
    event allsvr_output_done;
    int trans_num_total = 1;
    int trans_num_idx;
    int ap_done_cnt=1;
    event dut2tb_ap_ready;
    event dut2tb_ap_done;
    event ap_ready_for_nexttrans;
    event ap_done_for_nexttrans;
    event finish;
    Softmax_config Softmax_cfg;
    virtual interface misc_interface misc_if;

    mem_model_pages#(32,8) mem_blk_pages_CTRL_BUS_seq_len;
    int svr_in_stream_delay;
    covergroup svr_in_stream_cov;
        delay: coverpoint svr_in_stream_delay
        {
            bins norm[257] = { [0 : 256] };
        }
    endgroup
    int svr_out_stream_delay;
    covergroup svr_out_stream_cov;
        delay: coverpoint svr_out_stream_delay
        {
            bins norm[257] = { [0 : 256] };
        }
    endgroup
    
    `uvm_component_utils_begin(Softmax_reference_model)
        `uvm_field_int (trans_num_idx, UVM_DEFAULT)
    `uvm_component_utils_end

    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if(!uvm_config_db#(virtual misc_interface)::get(this, "", "misc_if", misc_if))
            `uvm_fatal(this.get_full_name(), "No misc_if from high level")
    endfunction

    function new (string name = "", uvm_component parent = null);
        super.new (name, parent);
        svr_in_stream_cov = new;
        svr_out_stream_cov = new;
        trans_num_idx= 0;
    endfunction

    virtual task run_phase(uvm_phase phase);
        string fpath[$];

        fpath.push_back(`TV_IN_seq_len);
        mem_blk_pages_CTRL_BUS_seq_len = mem_model_pages#(32,8)::type_id::create("mem_blk_pages_CTRL_BUS_seq_len");
        mem_blk_pages_CTRL_BUS_seq_len.tvinload_pagechk_atinit(fpath, 1*((32+7)/8), 0, 16, "");
        fpath.delete;

        fork
            forever begin
                wait(write_data_finish_CTRL_BUS);
                `uvm_info("", "trigger_allaxilite_data_write_finish", UVM_LOW)
                @(posedge misc_if.clock);
                write_data_finish_CTRL_BUS = 0;
                -> allaxilite_write_data_finish;
            end
            forever begin
                //this is non-pipeline case
                forever begin
                    @(negedge misc_if.clock);
                    if(misc_if.dut2tb_ap_done===1) break;
                end
                @(posedge misc_if.clock);
                @allaxilite_write_data_finish;
                @(posedge misc_if.clock);
                -> ap_ready_for_nexttrans;
                `uvm_info(this.get_full_name(), "trigger event ap_ready_for_nexttrans", UVM_LOW)
                fork
                    begin
                        misc_if.ap_ready_for_nexttrans = 1;
                        @(posedge misc_if.clock);
                        misc_if.ap_ready_for_nexttrans = 0;
                    end
                join_none
            end
            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if(misc_if.dut2tb_ap_done===1) break;
                end
                @(posedge misc_if.clock);
                fork
                    begin
                        @(negedge misc_if.clock);
                        -> misc_if.dut2tb_ap_done_evt;
                        #0;
                        -> misc_if.dut2tb_ap_ready_evt;
                    end
                join_none
                -> ap_done_for_nexttrans;
                `uvm_info(this.get_full_name(), "trigger event ap_done_for_nexttrans", UVM_LOW)
                fork
                    begin
                        misc_if.ap_done_for_nexttrans = 1;
                        @(posedge misc_if.clock);
                        misc_if.ap_done_for_nexttrans = 0;
                    end
                join_none
            end

            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if (misc_if.dut2tb_ap_done===1)   break;
                end
                @(posedge misc_if.clock);
                -> dut2tb_ap_done;
                `uvm_info(this.get_full_name(), "trigger event DUT2TB_AP_DONE", UVM_LOW)
            end
            forever begin
                forever begin
                    @(negedge misc_if.clock);
                    if (misc_if.dut2tb_ap_ready === 1)   break;
                end
                @(posedge misc_if.clock);
                `uvm_info(this.get_full_name(), "trigger event DUT2TB_AP_READY", UVM_LOW)
                -> dut2tb_ap_ready;
            end
        join
    endtask

    virtual function void write_svr_master_in_stream(svr_transfer#(16) tr);
    //  trans_size++;
        svr_in_stream_delay = tr.delay;
        svr_in_stream_cov.sample();
        `uvm_info(this.get_full_name(), "port a collected one pkt", UVM_DEBUG);
    endfunction

    virtual function void write_svr_slave_out_stream(svr_transfer#(16) tr);
    //  trans_size++;
        svr_out_stream_delay = tr.delay;
        svr_out_stream_cov.sample();
        `uvm_info(this.get_full_name(), "port a collected one pkt", UVM_DEBUG);
    endfunction

    virtual function void write_axi_wtr_CTRL_BUS(axi_pkg::axi_transfer tr);
    endfunction
    virtual function void write_axi_rtr_CTRL_BUS(axi_pkg::axi_transfer tr);
            `uvm_info("receive axi read data", tr.sprint(), UVM_HIGH)
        if(0) begin //no block ctrl register processing
        end else begin
        end
    endfunction
endclass
`endif
