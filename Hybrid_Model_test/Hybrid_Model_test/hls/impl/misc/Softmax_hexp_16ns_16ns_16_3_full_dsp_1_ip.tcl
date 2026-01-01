# BEGIN Vivado Commands 
create_ip -name floating_point -version 7.1 -vendor xilinx.com -library ip -module_name Softmax_hexp_16ns_16ns_16_3_full_dsp_1_ip
# BEGIN Vivado Commands 
# BEGIN Vivado Parameters
set_property -dict [list CONFIG.a_precision_type Half \
                          CONFIG.c_a_exponent_width 5 \
                          CONFIG.c_a_fraction_width 11 \
                          CONFIG.c_latency 2 \
                          CONFIG.c_mult_usage Full_Usage \
                          CONFIG.c_rate 1 \
                          CONFIG.c_result_exponent_width 5 \
                          CONFIG.c_result_fraction_width 11 \
                          CONFIG.component_name Softmax_hexp_16ns_16ns_16_3_full_dsp_1_ip \
                          CONFIG.flow_control NonBlocking \
                          CONFIG.has_aclken true \
                          CONFIG.has_result_tready false \
                          CONFIG.maximum_latency false \
                          CONFIG.operation_type Exponential \
                          CONFIG.result_precision_type Half ] -objects [get_ips Softmax_hexp_16ns_16ns_16_3_full_dsp_1_ip] -quiet
# END Vivado Parameters
generate_target {synthesis simulation} [get_files Softmax_hexp_16ns_16ns_16_3_full_dsp_1_ip.xci]
