// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

// CHECK: hl.var "fp16" : !hl.lvalue<!hl.half>
__fp16 fp16 = 0.5;

// CHECK: hl.var "f" : !hl.lvalue<!hl.float>
// CHECK: hl.const #hl.float<5.000000e-01> : !hl.float
float f = 0.5f;

// CHECK: hl.var "d" : !hl.lvalue<!hl.double>
// CHECK: hl.const #hl.float<5.000000e-01> : !hl.double
double d = 0.5;

// CHECK: hl.var "ld" : !hl.lvalue<!hl.longdouble>
// CHECK: hl.const #hl.float<5.000000e-01> : !hl.longdouble
long double ld = 0.5L;
