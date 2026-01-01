`timescale 1 ns / 1 ps

module AESL_deadlock_kernel_monitor_top ( 
    input wire kernel_monitor_clock,
    input wire kernel_monitor_reset
);
wire [1:0] axis_block_sigs;
wire [8:0] inst_idle_sigs;
wire [3:0] inst_block_sigs;
wire kernel_block;

assign axis_block_sigs[0] = ~AESL_inst_Softmax.Loop_max_value_proc_U0.grp_Loop_max_value_proc_Pipeline_VITIS_LOOP_41_1_fu_101.in_stream_TDATA_blk_n;
assign axis_block_sigs[1] = ~AESL_inst_Softmax.Loop_softmax_output_proc_U0.grp_Loop_softmax_output_proc_Pipeline_VITIS_LOOP_86_3_fu_82.out_stream_TDATA_blk_n;

assign inst_idle_sigs[0] = AESL_inst_Softmax.Loop_max_value_proc_U0.ap_idle;
assign inst_block_sigs[0] = (AESL_inst_Softmax.Loop_max_value_proc_U0.ap_done & ~AESL_inst_Softmax.Loop_max_value_proc_U0.ap_continue) | ~AESL_inst_Softmax.Loop_max_value_proc_U0.max_value_stream_blk_n | ~AESL_inst_Softmax.Loop_max_value_proc_U0.grp_Loop_max_value_proc_Pipeline_VITIS_LOOP_41_1_fu_101.max_bypass_stream_blk_n | ~AESL_inst_Softmax.Loop_max_value_proc_U0.seq_len_c1_blk_n;
assign inst_idle_sigs[1] = AESL_inst_Softmax.Loop_sub_max_proc_U0.ap_idle;
assign inst_block_sigs[1] = (AESL_inst_Softmax.Loop_sub_max_proc_U0.ap_done & ~AESL_inst_Softmax.Loop_sub_max_proc_U0.ap_continue) | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.max_value_stream_blk_n | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.seq_len_blk_n | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.sum_exp_stream_blk_n | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.grp_Loop_sub_max_proc_Pipeline_VITIS_LOOP_57_2_fu_161.max_bypass_stream_blk_n | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.grp_Loop_sub_max_proc_Pipeline_VITIS_LOOP_57_2_fu_161.exp_bypass_stream_blk_n | ~AESL_inst_Softmax.Loop_sub_max_proc_U0.seq_len_c_blk_n;
assign inst_idle_sigs[2] = AESL_inst_Softmax.Loop_divide_proc_U0.ap_idle;
assign inst_block_sigs[2] = (AESL_inst_Softmax.Loop_divide_proc_U0.ap_done & ~AESL_inst_Softmax.Loop_divide_proc_U0.ap_continue) | ~AESL_inst_Softmax.Loop_divide_proc_U0.sum_exp_stream_blk_n | ~AESL_inst_Softmax.Loop_divide_proc_U0.inv_sum_exp_stream_blk_n;
assign inst_idle_sigs[3] = AESL_inst_Softmax.Loop_softmax_output_proc_U0.ap_idle;
assign inst_block_sigs[3] = (AESL_inst_Softmax.Loop_softmax_output_proc_U0.ap_done & ~AESL_inst_Softmax.Loop_softmax_output_proc_U0.ap_continue) | ~AESL_inst_Softmax.Loop_softmax_output_proc_U0.inv_sum_exp_stream_blk_n | ~AESL_inst_Softmax.Loop_softmax_output_proc_U0.seq_len_blk_n | ~AESL_inst_Softmax.Loop_softmax_output_proc_U0.grp_Loop_softmax_output_proc_Pipeline_VITIS_LOOP_86_3_fu_82.exp_bypass_stream_blk_n;

assign inst_idle_sigs[4] = 1'b0;
assign inst_idle_sigs[5] = AESL_inst_Softmax.Loop_max_value_proc_U0.ap_idle;
assign inst_idle_sigs[6] = AESL_inst_Softmax.Loop_max_value_proc_U0.grp_Loop_max_value_proc_Pipeline_VITIS_LOOP_41_1_fu_101.ap_idle;
assign inst_idle_sigs[7] = AESL_inst_Softmax.Loop_softmax_output_proc_U0.ap_idle;
assign inst_idle_sigs[8] = AESL_inst_Softmax.Loop_softmax_output_proc_U0.grp_Loop_softmax_output_proc_Pipeline_VITIS_LOOP_86_3_fu_82.ap_idle;

AESL_deadlock_idx0_monitor AESL_deadlock_idx0_monitor_U (
    .clock(kernel_monitor_clock),
    .reset(kernel_monitor_reset),
    .axis_block_sigs(axis_block_sigs),
    .inst_idle_sigs(inst_idle_sigs),
    .inst_block_sigs(inst_block_sigs),
    .block(kernel_block)
);


initial begin : trigger_axis_deadlock
reg block_delay;
    block_delay = 0;
    while(1) begin
        @(posedge kernel_monitor_clock);
    if (kernel_block == 1'b1 && block_delay == 1'b0)
        block_delay = kernel_block;
    end
end

endmodule
