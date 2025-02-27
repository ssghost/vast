// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

// adapted from https://gist.github.com/fay59/5ccbe684e6e56a7df8815c3486568f01

// CHECK: hl.func external @foo (%arg0: !hl.lvalue<!hl.decayed<!hl.ptr<!hl.int,  const, volatile, restrict >>>)
void foo(int arr[static const restrict volatile 10]) {
    // static: the array contains at least 10 elements
    // const, volatile and restrict all apply to the array type.
}
