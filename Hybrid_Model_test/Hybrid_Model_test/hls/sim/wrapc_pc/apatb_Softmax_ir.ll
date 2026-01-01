; ModuleID = '/home/percy/data/HDL/HybridModelTest/Hybrid_Model_test/Hybrid_Model_test/hls/.autopilot/db/a.g.ld.5.gdce.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-i64:64-i128:128-i256:256-i512:512-i1024:1024-i2048:2048-i4096:4096-n8:16:32:64-S128-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024"
target triple = "fpga64-xilinx-none"

%"class.hls::stream<__fp16, 0>" = type { half }

; Function Attrs: inaccessiblememonly nounwind willreturn
declare void @llvm.sideeffect() #0

; Function Attrs: inaccessiblemem_or_argmemonly noinline willreturn
define void @apatb_Softmax_ir(%"class.hls::stream<__fp16, 0>"* noalias nocapture nonnull dereferenceable(2) %in_stream, %"class.hls::stream<__fp16, 0>"* noalias nocapture nonnull dereferenceable(2) %out_stream, i32 %seq_len) local_unnamed_addr #1 {
entry:
  %in_stream_copy = alloca half, align 512
  call void @llvm.sideeffect() #7 [ "stream_interface"(half* %in_stream_copy, i32 0) ]
  %out_stream_copy = alloca half, align 512
  call void @llvm.sideeffect() #7 [ "stream_interface"(half* %out_stream_copy, i32 0) ]
  call fastcc void @copy_in(%"class.hls::stream<__fp16, 0>"* nonnull %in_stream, half* nonnull align 512 %in_stream_copy, %"class.hls::stream<__fp16, 0>"* nonnull %out_stream, half* nonnull align 512 %out_stream_copy)
  call void @apatb_Softmax_hw(half* %in_stream_copy, half* %out_stream_copy, i32 %seq_len)
  call void @copy_back(%"class.hls::stream<__fp16, 0>"* %in_stream, half* %in_stream_copy, %"class.hls::stream<__fp16, 0>"* %out_stream, half* %out_stream_copy)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_in(%"class.hls::stream<__fp16, 0>"* noalias "unpacked"="0", half* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<__fp16, 0>"* noalias "unpacked"="2", half* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #2 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>.16"(half* align 512 %1, %"class.hls::stream<__fp16, 0>"* %0)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>.16"(half* align 512 %3, %"class.hls::stream<__fp16, 0>"* %2)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_out(%"class.hls::stream<__fp16, 0>"* noalias "unpacked"="0", half* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<__fp16, 0>"* noalias "unpacked"="2", half* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>"(%"class.hls::stream<__fp16, 0>"* %0, half* align 512 %1)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>"(%"class.hls::stream<__fp16, 0>"* %2, half* align 512 %3)
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>"(%"class.hls::stream<__fp16, 0>"* noalias "unpacked"="0" %dst, half* noalias nocapture align 512 "unpacked"="1.0" %src) unnamed_addr #4 {
entry:
  %0 = icmp eq %"class.hls::stream<__fp16, 0>"* %dst, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<__fp16, 0>.11"(%"class.hls::stream<__fp16, 0>"* nonnull %dst, half* align 512 %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<__fp16, 0>.11"(%"class.hls::stream<__fp16, 0>"* noalias nocapture "unpacked"="0", half* noalias nocapture align 512 "unpacked"="1.0") unnamed_addr #5 {
entry:
  %2 = alloca half
  %3 = alloca %"class.hls::stream<__fp16, 0>"
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast half* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_2(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast half* %2 to i8*
  %7 = bitcast half* %1 to i8*
  call void @fpga_fifo_pop_2(i8* %6, i8* %7)
  %8 = load volatile half, half* %2
  %.ivi = insertvalue %"class.hls::stream<__fp16, 0>" undef, half %8, 0
  store %"class.hls::stream<__fp16, 0>" %.ivi, %"class.hls::stream<__fp16, 0>"* %3
  %9 = bitcast %"class.hls::stream<__fp16, 0>"* %3 to i8*
  %10 = bitcast %"class.hls::stream<__fp16, 0>"* %0 to i8*
  call void @fpga_fifo_push_2(i8* %9, i8* %10)
  br label %empty, !llvm.loop !5

ret:                                              ; preds = %empty
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>.16"(half* noalias nocapture align 512 "unpacked"="0.0" %dst, %"class.hls::stream<__fp16, 0>"* noalias "unpacked"="1" %src) unnamed_addr #4 {
entry:
  %0 = icmp eq %"class.hls::stream<__fp16, 0>"* %src, null
  br i1 %0, label %ret, label %copy

copy:                                             ; preds = %entry
  call fastcc void @"streamcpy_hls.p0class.hls::stream<__fp16, 0>.19"(half* align 512 %dst, %"class.hls::stream<__fp16, 0>"* nonnull %src)
  br label %ret

ret:                                              ; preds = %copy, %entry
  ret void
}

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @"streamcpy_hls.p0class.hls::stream<__fp16, 0>.19"(half* noalias nocapture align 512 "unpacked"="0.0", %"class.hls::stream<__fp16, 0>"* noalias nocapture "unpacked"="1") unnamed_addr #5 {
entry:
  %2 = alloca %"class.hls::stream<__fp16, 0>"
  %3 = alloca half
  br label %empty

empty:                                            ; preds = %push, %entry
  %4 = bitcast %"class.hls::stream<__fp16, 0>"* %1 to i8*
  %5 = call i1 @fpga_fifo_not_empty_2(i8* %4)
  br i1 %5, label %push, label %ret

push:                                             ; preds = %empty
  %6 = bitcast %"class.hls::stream<__fp16, 0>"* %2 to i8*
  %7 = bitcast %"class.hls::stream<__fp16, 0>"* %1 to i8*
  call void @fpga_fifo_pop_2(i8* %6, i8* %7)
  %8 = load volatile %"class.hls::stream<__fp16, 0>", %"class.hls::stream<__fp16, 0>"* %2
  %.evi = extractvalue %"class.hls::stream<__fp16, 0>" %8, 0
  store half %.evi, half* %3
  %9 = bitcast half* %3 to i8*
  %10 = bitcast half* %0 to i8*
  call void @fpga_fifo_push_2(i8* %9, i8* %10)
  br label %empty, !llvm.loop !7

ret:                                              ; preds = %empty
  ret void
}

declare i8* @malloc(i64)

declare void @free(i8*)

declare void @apatb_Softmax_hw(half*, half*, i32)

; Function Attrs: argmemonly noinline willreturn
define internal fastcc void @copy_back(%"class.hls::stream<__fp16, 0>"* noalias "unpacked"="0", half* noalias nocapture align 512 "unpacked"="1.0", %"class.hls::stream<__fp16, 0>"* noalias "unpacked"="2", half* noalias nocapture align 512 "unpacked"="3.0") unnamed_addr #3 {
entry:
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>"(%"class.hls::stream<__fp16, 0>"* %0, half* align 512 %1)
  call fastcc void @"onebyonecpy_hls.p0class.hls::stream<__fp16, 0>"(%"class.hls::stream<__fp16, 0>"* %2, half* align 512 %3)
  ret void
}

declare void @Softmax_hw_stub(%"class.hls::stream<__fp16, 0>"* noalias nocapture nonnull, %"class.hls::stream<__fp16, 0>"* noalias nocapture nonnull, i32)

define void @Softmax_hw_stub_wrapper(half*, half*, i32) #6 {
entry:
  %3 = call i8* @malloc(i64 2)
  %4 = bitcast i8* %3 to %"class.hls::stream<__fp16, 0>"*
  %5 = call i8* @malloc(i64 2)
  %6 = bitcast i8* %5 to %"class.hls::stream<__fp16, 0>"*
  call void @copy_out(%"class.hls::stream<__fp16, 0>"* %4, half* %0, %"class.hls::stream<__fp16, 0>"* %6, half* %1)
  call void @Softmax_hw_stub(%"class.hls::stream<__fp16, 0>"* %4, %"class.hls::stream<__fp16, 0>"* %6, i32 %2)
  call void @copy_in(%"class.hls::stream<__fp16, 0>"* %4, half* %0, %"class.hls::stream<__fp16, 0>"* %6, half* %1)
  call void @free(i8* %3)
  call void @free(i8* %5)
  ret void
}

declare i1 @fpga_fifo_not_empty_2(i8*)

declare void @fpga_fifo_pop_2(i8*, i8*)

declare void @fpga_fifo_push_2(i8*, i8*)

attributes #0 = { inaccessiblememonly nounwind willreturn }
attributes #1 = { inaccessiblemem_or_argmemonly noinline willreturn "fpga.wrapper.func"="wrapper" }
attributes #2 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyin" }
attributes #3 = { argmemonly noinline willreturn "fpga.wrapper.func"="copyout" }
attributes #4 = { argmemonly noinline willreturn "fpga.wrapper.func"="onebyonecpy_hls" }
attributes #5 = { argmemonly noinline willreturn "fpga.wrapper.func"="streamcpy_hls" }
attributes #6 = { "fpga.wrapper.func"="stub" }
attributes #7 = { inaccessiblememonly nounwind willreturn "xlx.port.bitwidth"="16" "xlx.source"="user" }

!llvm.dbg.cu = !{}
!llvm.ident = !{!0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0, !0}
!llvm.module.flags = !{!1, !2, !3}
!blackbox_cfg = !{!4}

!0 = !{!"clang version 7.0.0 "}
!1 = !{i32 2, !"Dwarf Version", i32 4}
!2 = !{i32 2, !"Debug Info Version", i32 3}
!3 = !{i32 1, !"wchar_size", i32 4}
!4 = !{}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.rotate.disable"}
!7 = distinct !{!7, !6}
