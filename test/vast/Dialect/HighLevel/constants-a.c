// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

// CHECK: hl.var "i" : !hl.lvalue<!hl.int>
// CHECK:   hl.const #hl.integer<0> : !hl.int
int i = 0;

// CHECK: hl.var "ui" : !hl.lvalue<!hl.int< unsigned >>
// CHECK:   hl.const #hl.integer<0> : !hl.int< unsigned >
unsigned int ui = 0u;

// CHECK: hl.var "lli" : !hl.lvalue<!hl.longlong>
// CHECK:   hl.const #hl.integer<0> : !hl.longlong
long long int lli = 0ll;

// CHECK: hl.var "f" : !hl.lvalue<!hl.float>
// CHECK:   hl.const #hl.float<0.000000e+00> : !hl.float
float f = 0.f;

// CHECK: hl.var "d" : !hl.lvalue<!hl.double>
// CHECK:   hl.const #hl.float<0.000000e+00> : !hl.double
double d = 0.0;

// CHECK: hl.var "str" : !hl.lvalue<!hl.ptr<!hl.char< const >>>
// CHECK: hl.const #hl.strlit<"hello"> : !hl.lvalue<!hl.array<6, !hl.char>>
const char *str = "hello";

// CHECK: hl.var "arr" : !hl.lvalue<!hl.array<3, !hl.int< const >>>
// CHECK:   hl.const #hl.integer<1> : !hl.int
// CHECK:   hl.const #hl.integer<2> : !hl.int
// CHECK:   hl.const #hl.integer<3> : !hl.int
const int arr[] = { 1, 2, 3 };
