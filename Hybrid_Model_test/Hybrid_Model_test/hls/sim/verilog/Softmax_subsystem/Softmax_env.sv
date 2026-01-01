//==============================================================
//Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
//Tool Version Limit: 2025.05
//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
//
//==============================================================
`ifndef SOFTMAX_ENV__SV                                                                                   
    `define SOFTMAX_ENV__SV                                                                               
                                                                                                                    
                                                                                                                    
    class Softmax_env extends uvm_env;                                                                          
                                                                                                                    
        Softmax_virtual_sequencer Softmax_virtual_sqr;                                                      
        Softmax_config Softmax_cfg;                                                                         
                                                                                                                    
        svr_pkg::svr_env#(16) env_master_svr_in_stream;
        svr_pkg::svr_env#(16) env_slave_svr_out_stream;
        axi_pkg::axi_env#(5,4,4,3,1) axi_lite_CTRL_BUS;
                                                                                                                    
        Softmax_reference_model   refm;                                                                         
                                                                                                                    
        Softmax_subsystem_monitor subsys_mon;                                                                   
                                                                                                                    
        `uvm_component_utils_begin(Softmax_env)                                                                 
        `uvm_field_object (env_master_svr_in_stream,  UVM_DEFAULT | UVM_REFERENCE)
        `uvm_field_object (env_slave_svr_out_stream,  UVM_DEFAULT | UVM_REFERENCE)
        `uvm_field_object (refm, UVM_DEFAULT | UVM_REFERENCE)                                                       
        `uvm_field_object (Softmax_virtual_sqr, UVM_DEFAULT | UVM_REFERENCE)                                    
        `uvm_field_object (Softmax_cfg        , UVM_DEFAULT)                                                    
        `uvm_component_utils_end                                                                                    
                                                                                                                    
        function new (string name = "Softmax_env", uvm_component parent = null);                              
            super.new(name, parent);                                                                                
        endfunction                                                                                                 
                                                                                                                    
        extern virtual function void build_phase(uvm_phase phase);                                                  
        extern virtual function void connect_phase(uvm_phase phase);                                                
        extern virtual task          run_phase(uvm_phase phase);                                                    
                                                                                                                    
    endclass                                                                                                        
                                                                                                                    
    function void Softmax_env::build_phase(uvm_phase phase);                                                    
        super.build_phase(phase);                                                                                   
        Softmax_cfg = Softmax_config::type_id::create("Softmax_cfg", this);                           
                                                                                                                    
        Softmax_cfg.port_in_stream_cfg.svr_type = svr_pkg::SVR_MASTER ;
        env_master_svr_in_stream  = svr_env#(16)::type_id::create("env_master_svr_in_stream", this);
        uvm_config_db#(svr_pkg::svr_config)::set(this, "env_master_svr_in_stream*", "cfg", Softmax_cfg.port_in_stream_cfg);
        Softmax_cfg.port_in_stream_cfg.prt_type = svr_pkg::AXIS;
        Softmax_cfg.port_in_stream_cfg.is_active = svr_pkg::SVR_ACTIVE;
        Softmax_cfg.port_in_stream_cfg.spec_cfg = svr_pkg::NORMAL;
        Softmax_cfg.port_in_stream_cfg.reset_level = svr_pkg::RESET_LEVEL_LOW;
 
        Softmax_cfg.port_out_stream_cfg.svr_type = svr_pkg::SVR_SLAVE ;
        env_slave_svr_out_stream  = svr_env#(16)::type_id::create("env_slave_svr_out_stream", this);
        uvm_config_db#(svr_pkg::svr_config)::set(this, "env_slave_svr_out_stream*", "cfg", Softmax_cfg.port_out_stream_cfg);
        Softmax_cfg.port_out_stream_cfg.prt_type = svr_pkg::AXIS;
        Softmax_cfg.port_out_stream_cfg.is_active = svr_pkg::SVR_ACTIVE;
        Softmax_cfg.port_out_stream_cfg.spec_cfg = svr_pkg::NORMAL;
        Softmax_cfg.port_out_stream_cfg.reset_level = svr_pkg::RESET_LEVEL_LOW;
 

        Softmax_cfg.CTRL_BUS_cfg.set_default();
        Softmax_cfg.CTRL_BUS_cfg.drv_type = axi_pkg::MASTER;
        Softmax_cfg.CTRL_BUS_cfg.reset_level = axi_pkg::RESET_LEVEL_LOW;
        uvm_config_db#(axi_pkg::axi_cfg)::set(this, "axi_lite_CTRL_BUS*", "cfg", Softmax_cfg.CTRL_BUS_cfg);
        axi_lite_CTRL_BUS = axi_pkg::axi_env#(5,4,4,3,1)::type_id::create("axi_lite_CTRL_BUS", this);



        refm = Softmax_reference_model::type_id::create("refm", this);


        uvm_config_db#(Softmax_reference_model)::set(this, "*", "refm", refm);


        `uvm_info(this.get_full_name(), "set reference model by uvm_config_db", UVM_LOW)


        subsys_mon = Softmax_subsystem_monitor::type_id::create("subsys_mon", this);


        Softmax_virtual_sqr = Softmax_virtual_sequencer::type_id::create("Softmax_virtual_sqr", this);
        `uvm_info(this.get_full_name(), "build_phase done", UVM_LOW)
    endfunction


    function void Softmax_env::connect_phase(uvm_phase phase);
        super.connect_phase(phase);


        Softmax_virtual_sqr.svr_port_in_stream_sqr = env_master_svr_in_stream.m_agt.sqr;
        env_master_svr_in_stream.m_agt.mon.item_collect_port.connect(subsys_mon.svr_master_in_stream_imp);
 
        Softmax_virtual_sqr.svr_port_out_stream_sqr = env_slave_svr_out_stream.s_agt.sqr;
        env_slave_svr_out_stream.s_agt.mon.item_collect_port.connect(subsys_mon.svr_slave_out_stream_imp);
 
        if(Softmax_cfg.CTRL_BUS_cfg.drv_type==axi_pkg::MASTER ||Softmax_cfg.CTRL_BUS_cfg.drv_type==axi_pkg::SLAVE)
            Softmax_virtual_sqr.CTRL_BUS_sqr = axi_lite_CTRL_BUS.vsqr;
        axi_lite_CTRL_BUS.item_wtr_port.connect(subsys_mon.CTRL_BUS_wtr_imp);
        axi_lite_CTRL_BUS.item_rtr_port.connect(subsys_mon.CTRL_BUS_rtr_imp);
        refm.Softmax_cfg = Softmax_cfg;
        `uvm_info(this.get_full_name(), "connect phase done", UVM_LOW)
    endfunction


    task Softmax_env::run_phase(uvm_phase phase);
        `uvm_info(this.get_full_name(), "Softmax_env is running", UVM_LOW)
    endtask


`endif
