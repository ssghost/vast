// RUN: vast-cc --ccopts -xc --from-source %s | vast-opt --vast-hl-lower-types | FileCheck %s

// CHECK-LABEL: hl.func external @main () -> si32
int main()
{
    // CHECK: hl.var "x" : !hl.lvalue<si32> =  {
    // CHECK:   [[V1:%[0-9]+]] = hl.const #hl.integer<0> : si32
    // CHECK:   hl.value.yield [[V1]] : si32
    int x = 0;
    // CHECK: hl.var "cx" : !hl.lvalue<si32> =  {
    // CHECK:   [[V2:%[0-9]+]] = hl.const #hl.integer<0> : si32
    // CHECK:   hl.value.yield [[V2]] : si32
    const int cx = 0;
    // CHECK: hl.var "cvx" : !hl.lvalue<si32> =  {
    // CHECK:   [[V3:%[0-9]+]] = hl.const #hl.integer<0> : si32
    // CHECK:   hl.value.yield [[V3]] : si32
    const volatile int cvx = 0;
}
