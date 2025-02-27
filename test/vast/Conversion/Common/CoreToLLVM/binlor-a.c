// RUN: vast-cc --ccopts -xc --from-source %s | vast-opt --vast-hl-lower-types --vast-hl-to-ll-cf --vast-hl-to-ll-vars --vast-hl-to-lazy-regions --vast-irs-to-llvm --vast-core-to-llvm | FileCheck %s

int fun(int arg1, int arg2) {
    int res = arg1 || arg2;
    // CHECK: [[LHS:%[0-9]+]] = llvm.load [[V1:%[0-9]+]]
    // CHECK: [[Z:%[0-9]+]] = llvm.mlir.constant(0 : i32) : i32
    // CHECK: [[LR:%[0-9]+]] = llvm.icmp "ne" [[LHS]], [[Z]] : i32
    // CHECK: llvm.cond_br [[LR]], ^[[RBLOCK:bb[0-9]+]]([[LR]] : i1), ^[[FBLOCK:bb[0-9]+]]
    // CHECK: ^[[FBLOCK]]: // pred: ^[[PRED:bb[0-9]+]]
    // CHECK: [[RHS:%[0-9]+]] = llvm.load [[V2:%[0-9]+]]
    // CHECK: [[RR:%[0-9]+]] = llvm.icmp "ne" [[RHS]], [[Z]]
    // CHECK: llvm.br ^[[RBLOCK]]([[RR]] : i1)
    // CHECK: ^[[RBLOCK]]([[V3:%[0-9]+]]: i1): // 2 preds: ^[[PRED]], ^[[FBLOCK]]
    return res;
}
