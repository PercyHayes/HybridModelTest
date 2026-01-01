# ==============================================================
# Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
# Tool Version Limit: 2025.05
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
# 
# ==============================================================
CSIM_DESIGN = 1

__SIM_FPO__ = 1

__SIM_MATHHLS__ = 1

__SIM_FFT__ = 1

__SIM_FIR__ = 1

__SIM_DDS__ = 1

__USE_CLANG__ = 1

__USE_VCXX_CLANG__ = 1

ObjDir = obj

HLS_SOURCES = ../../../../../src/testbench/Softmax/tb_SoftMax.cpp ../../../../../src/main/SoftMax.cpp ../../../../../src/main/Weight_Loader.cpp ../../../../../src/main/Stream_Copy.cpp ../../../../../src/main/Scale_Loader_Distributor.cpp ../../../../../src/main/Gemv_Test.cpp ../../../../../src/main/Bias_Merger.cpp ../../../../../src/main/Accumulator_Quantizer.cpp ../../../../../src/main/Mul_Adder_Tree_128.cpp

override TARGET := csim.exe

AUTOPILOT_ROOT := /software/xilinx/2025.1.1/Vitis
AUTOPILOT_MACH := lnx64
ifdef AP_GCC_M32
  AUTOPILOT_MACH := Linux_x86
  IFLAG += -m32
endif
IFLAG += -fPIC
ifndef AP_GCC_PATH
  AP_GCC_PATH := /software/xilinx/2025.1.1/Vitis/tps/lnx64/gcc-8.3.0/bin
endif
AUTOPILOT_TOOL := ${AUTOPILOT_ROOT}/${AUTOPILOT_MACH}/tools
AP_CLANG_PATH := ${XILINX_VCXX}/libexec
AUTOPILOT_TECH := ${AUTOPILOT_ROOT}/common/technology


IFLAG += -I "${AUTOPILOT_ROOT}/include"
IFLAG += -I "${AUTOPILOT_ROOT}/include/ap_sysc"
IFLAG += -I "${AUTOPILOT_TECH}/generic/SystemC"
IFLAG += -I "${AUTOPILOT_TECH}/generic/SystemC/AESL_FP_comp"
IFLAG += -I "${AUTOPILOT_TECH}/generic/SystemC/AESL_comp"
IFLAG += -I "${AUTOPILOT_TOOL}/auto_cc/include"
IFLAG += -I "/usr/include/x86_64-linux-gnu"
IFLAG += -D__HLS_COSIM__

IFLAG += -D__HLS_CSIM__

IFLAG += -D__VITIS_HLS__

IFLAG += -D__SIM_FPO__

IFLAG += -D__SIM_FFT__

IFLAG += -D__SIM_FIR__

IFLAG += -D__SIM_DDS__

IFLAG += -D__DSP58_PRIMARY__
IFLAG += -g
DFLAG += -D__xilinx_ip_top= -DAESL_TB
CCFLAG += -Werror=return-type
CCFLAG += -Wno-abi
CCFLAG += -fdebug-default-version=4
CCFLAG += --gcc-toolchain=/software/xilinx/2025.1.1/Vitis/tps/lnx64/gcc-8.3.0
CCFLAG += -Werror=uninitialized
CCFLAG += -Wno-c++11-narrowing
CCFLAG += -Wno-error=sometimes-uninitialized
LFLAG += --gcc-toolchain=/software/xilinx/2025.1.1/Vitis/tps/lnx64/gcc-8.3.0



include ./Makefile.rules

all: $(TARGET)



$(ObjDir)/tb_SoftMax.o: ../../../../../src/testbench/Softmax/tb_SoftMax.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/testbench/Softmax/tb_SoftMax.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD -Wno-unknown-pragmas -Wno-unknown-pragmas  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/tb_SoftMax.d

$(ObjDir)/SoftMax.o: ../../../../../src/main/SoftMax.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/SoftMax.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/SoftMax.d

$(ObjDir)/Weight_Loader.o: ../../../../../src/main/Weight_Loader.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Weight_Loader.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Weight_Loader.d

$(ObjDir)/Stream_Copy.o: ../../../../../src/main/Stream_Copy.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Stream_Copy.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Stream_Copy.d

$(ObjDir)/Scale_Loader_Distributor.o: ../../../../../src/main/Scale_Loader_Distributor.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Scale_Loader_Distributor.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Scale_Loader_Distributor.d

$(ObjDir)/Gemv_Test.o: ../../../../../src/main/Gemv_Test.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Gemv_Test.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Gemv_Test.d

$(ObjDir)/Bias_Merger.o: ../../../../../src/main/Bias_Merger.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Bias_Merger.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Bias_Merger.d

$(ObjDir)/Accumulator_Quantizer.o: ../../../../../src/main/Accumulator_Quantizer.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Accumulator_Quantizer.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Accumulator_Quantizer.d

$(ObjDir)/Mul_Adder_Tree_128.o: ../../../../../src/main/Mul_Adder_Tree_128.cpp $(ObjDir)/.dir csim.mk
	$(Echo) "   Compiling ../../../../../src/main/Mul_Adder_Tree_128.cpp in $(BuildMode) mode" $(AVE_DIR_DLOG)
	$(Verb)  $(CXX) -std=gnu++14 ${CCFLAG} -c -MMD  $(IFLAG) $(DFLAG) $< -o $@ ; \

-include $(ObjDir)/Mul_Adder_Tree_128.d
