//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef SV_MODULE_TOP_SV
`define SV_MODULE_TOP_SV


`timescale 1ns/1ps


`include "uvm_macros.svh"
import uvm_pkg::*;
import file_agent_pkg::*;
import svr_pkg::*;
import Softmax_subsystem_pkg::*;
`include "Softmax_subsys_test_sequence_lib.sv"
`include "Softmax_test_lib.sv"


module sv_module_top;


    misc_interface              misc_if ( .clock(apatb_Softmax_top.AESL_clock), .reset(apatb_Softmax_top.AESL_reset) );
    assign apatb_Softmax_top.ap_start = misc_if.tb2dut_ap_start;
    assign misc_if.dut2tb_ap_done = apatb_Softmax_top.ap_done;
    assign misc_if.dut2tb_ap_ready = apatb_Softmax_top.ap_ready;
    initial begin
        uvm_config_db #(virtual misc_interface)::set(null, "uvm_test_top.top_env.*", "misc_if", misc_if);
    end


    svr_if #(16)  svr_in_stream_if    (.clk  (apatb_Softmax_top.AESL_clock), .rst(apatb_Softmax_top.AESL_reset));
    assign svr_in_stream_if.ready = apatb_Softmax_top.in_stream_TREADY;
    assign apatb_Softmax_top.in_stream_TVALID = svr_in_stream_if.valid;
    assign apatb_Softmax_top.in_stream_TDATA = svr_in_stream_if.data[15:0];
    initial begin
        uvm_config_db #( virtual svr_if#(16) )::set(null, "uvm_test_top.top_env.env_master_svr_in_stream.*", "vif", svr_in_stream_if);
    end


    svr_if #(16)  svr_out_stream_if    (.clk  (apatb_Softmax_top.AESL_clock), .rst(apatb_Softmax_top.AESL_reset));
    assign apatb_Softmax_top.out_stream_TREADY = svr_out_stream_if.ready;
    assign svr_out_stream_if.valid = apatb_Softmax_top.out_stream_TVALID;
    assign svr_out_stream_if.data[15:0] = apatb_Softmax_top.out_stream_TDATA;
    initial begin
        uvm_config_db #( virtual svr_if#(16) )::set(null, "uvm_test_top.top_env.env_slave_svr_out_stream.*", "vif", svr_out_stream_if);
    end


    axi_if #(5,4,4,3,1)  axi_CTRL_BUS_if (.clk  (apatb_Softmax_top.AESL_clock), .rst(apatb_Softmax_top.AESL_reset));
    assign apatb_Softmax_top.CTRL_BUS_AWADDR = axi_CTRL_BUS_if.AWADDR;
    assign apatb_Softmax_top.CTRL_BUS_AWVALID = axi_CTRL_BUS_if.AWVALID;
    assign axi_CTRL_BUS_if.AWREADY = apatb_Softmax_top.CTRL_BUS_AWREADY;
    assign apatb_Softmax_top.CTRL_BUS_WVALID = axi_CTRL_BUS_if.WVALID;
    assign axi_CTRL_BUS_if.WREADY = apatb_Softmax_top.CTRL_BUS_WREADY;
    assign apatb_Softmax_top.CTRL_BUS_WDATA = axi_CTRL_BUS_if.WDATA;
    assign apatb_Softmax_top.CTRL_BUS_WSTRB = axi_CTRL_BUS_if.WSTRB;
    assign apatb_Softmax_top.CTRL_BUS_ARADDR = axi_CTRL_BUS_if.ARADDR;
    assign apatb_Softmax_top.CTRL_BUS_ARVALID = axi_CTRL_BUS_if.ARVALID;
    assign axi_CTRL_BUS_if.ARREADY = apatb_Softmax_top.CTRL_BUS_ARREADY;
    assign axi_CTRL_BUS_if.RVALID = apatb_Softmax_top.CTRL_BUS_RVALID;
    assign apatb_Softmax_top.CTRL_BUS_RREADY = axi_CTRL_BUS_if.RREADY;
    assign axi_CTRL_BUS_if.RDATA = apatb_Softmax_top.CTRL_BUS_RDATA;
    assign axi_CTRL_BUS_if.RRESP = apatb_Softmax_top.CTRL_BUS_RRESP;
    assign axi_CTRL_BUS_if.BVALID = apatb_Softmax_top.CTRL_BUS_BVALID;
    assign apatb_Softmax_top.CTRL_BUS_BREADY = axi_CTRL_BUS_if.BREADY;
    assign axi_CTRL_BUS_if.BRESP = apatb_Softmax_top.CTRL_BUS_BRESP;
    assign axi_CTRL_BUS_if.BID = 0;
    assign axi_CTRL_BUS_if.RID = 0;
    assign axi_CTRL_BUS_if.RLAST = 1;
    initial begin
        uvm_config_db #( virtual axi_if#(5,4,4,3,1) )::set(null, "uvm_test_top.top_env.axi_lite_CTRL_BUS.*", "vif", axi_CTRL_BUS_if);
    end


    initial begin
        run_test();
    end
endmodule
`endif
