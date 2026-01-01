// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2025.1.1 (64-bit)
// Tool Version Limit: 2025.05
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// 
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#ifdef SDT
#include "xparameters.h"
#endif
#include "xsoftmax.h"

extern XSoftmax_Config XSoftmax_ConfigTable[];

#ifdef SDT
XSoftmax_Config *XSoftmax_LookupConfig(UINTPTR BaseAddress) {
	XSoftmax_Config *ConfigPtr = NULL;

	int Index;

	for (Index = (u32)0x0; XSoftmax_ConfigTable[Index].Name != NULL; Index++) {
		if (!BaseAddress || XSoftmax_ConfigTable[Index].Ctrl_bus_BaseAddress == BaseAddress) {
			ConfigPtr = &XSoftmax_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XSoftmax_Initialize(XSoftmax *InstancePtr, UINTPTR BaseAddress) {
	XSoftmax_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XSoftmax_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XSoftmax_CfgInitialize(InstancePtr, ConfigPtr);
}
#else
XSoftmax_Config *XSoftmax_LookupConfig(u16 DeviceId) {
	XSoftmax_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XSOFTMAX_NUM_INSTANCES; Index++) {
		if (XSoftmax_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XSoftmax_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XSoftmax_Initialize(XSoftmax *InstancePtr, u16 DeviceId) {
	XSoftmax_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XSoftmax_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XSoftmax_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif

#endif

