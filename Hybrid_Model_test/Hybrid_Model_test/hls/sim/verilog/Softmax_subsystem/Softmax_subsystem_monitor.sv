//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================

`ifndef SOFTMAX_SUBSYSTEM_MONITOR_SV
`define SOFTMAX_SUBSYSTEM_MONITOR_SV

`uvm_analysis_imp_decl(_svr_master_in_stream)
`uvm_analysis_imp_decl(_svr_slave_out_stream)
`uvm_analysis_imp_decl(_axi_wtr_CTRL_BUS)
`uvm_analysis_imp_decl(_axi_rtr_CTRL_BUS)

class Softmax_subsystem_monitor extends uvm_component;

    Softmax_reference_model refm;
    Softmax_scoreboard scbd;

    `uvm_component_utils_begin(Softmax_subsystem_monitor)
    `uvm_component_utils_end

    uvm_analysis_imp_svr_master_in_stream#(svr_pkg::svr_transfer#(16), Softmax_subsystem_monitor) svr_master_in_stream_imp;
    uvm_analysis_imp_svr_slave_out_stream#(svr_pkg::svr_transfer#(16), Softmax_subsystem_monitor) svr_slave_out_stream_imp;
    uvm_analysis_imp_axi_wtr_CTRL_BUS#(axi_pkg::axi_transfer, Softmax_subsystem_monitor) CTRL_BUS_wtr_imp;
    uvm_analysis_imp_axi_rtr_CTRL_BUS#(axi_pkg::axi_transfer, Softmax_subsystem_monitor) CTRL_BUS_rtr_imp;

    virtual function void build_phase(uvm_phase phase);
        super.build_phase(phase);
        if (!uvm_config_db#(Softmax_reference_model)::get(this, "", "refm", refm))
            `uvm_fatal(this.get_full_name(), "No refm from high level")
        `uvm_info(this.get_full_name(), "get reference model by uvm_config_db", UVM_MEDIUM)
        scbd = Softmax_scoreboard::type_id::create("scbd", this);
    endfunction

    virtual function void connect_phase(uvm_phase phase);
        super.connect_phase(phase);
    endfunction

    function new (string name = "", uvm_component parent = null);
        super.new(name, parent);
        svr_master_in_stream_imp = new("svr_master_in_stream_imp", this);
        svr_slave_out_stream_imp = new("svr_slave_out_stream_imp", this);
        CTRL_BUS_wtr_imp = new("CTRL_BUS_wtr_imp", this);
        CTRL_BUS_rtr_imp = new("CTRL_BUS_rtr_imp", this);
    endfunction

    virtual function void write_svr_master_in_stream(svr_transfer#(16) tr);
        refm.write_svr_master_in_stream(tr);
        scbd.write_svr_master_in_stream(tr);
    endfunction

    virtual function void write_svr_slave_out_stream(svr_transfer#(16) tr);
        refm.write_svr_slave_out_stream(tr);
        scbd.write_svr_slave_out_stream(tr);
    endfunction

    virtual function void write_axi_wtr_CTRL_BUS(axi_transfer tr);
        refm.write_axi_wtr_CTRL_BUS(tr);
        scbd.write_axi_wtr_CTRL_BUS(tr);
    endfunction

    virtual function void write_axi_rtr_CTRL_BUS(axi_transfer tr);
        refm.write_axi_rtr_CTRL_BUS(tr);
        scbd.write_axi_rtr_CTRL_BUS(tr);
    endfunction
endclass
`endif
