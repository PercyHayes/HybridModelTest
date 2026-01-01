// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
// Tool Version Limit: 2025.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
/***************************** Include Files *********************************/
#include "xsoftmax.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XSoftmax_CfgInitialize(XSoftmax *InstancePtr, XSoftmax_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Ctrl_bus_BaseAddress = ConfigPtr->Ctrl_bus_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XSoftmax_Set_seq_len(XSoftmax *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSoftmax_WriteReg(InstancePtr->Ctrl_bus_BaseAddress, XSOFTMAX_CTRL_BUS_ADDR_SEQ_LEN_DATA, Data);
}

u32 XSoftmax_Get_seq_len(XSoftmax *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSoftmax_ReadReg(InstancePtr->Ctrl_bus_BaseAddress, XSOFTMAX_CTRL_BUS_ADDR_SEQ_LEN_DATA);
    return Data;
}

