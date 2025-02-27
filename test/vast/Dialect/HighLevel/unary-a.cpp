// RUN: vast-cc --from-source %s | FileCheck %s
// RUN: vast-cc --from-source %s > %t && vast-opt %t | diff -B %t -

// CHECK-LABEL: hl.func external @_Z10arithmetici
void arithmetic(int a)
{
    // CHECK: hl.post.inc
    a++;
    // CHECK: hl.pre.inc
    ++a;
    // CHECK: hl.post.dec
    a--;
    // CHECK: hl.pre.dec
    --a;
}

// CHECK-LABEL: hl.func external @_Z4signi
void sign(int a)
{
    // CHECK: hl.plus
    +a;
    // CHECK: hl.minus
    -a;
}

// CHECK-LABEL: hl.func external @_Z6binaryi
void binary(int a)
{
    // CHECK: hl.not
    ~a;
}

// CHECK-LABEL: hl.func external @_Z7logicalb
void logical(bool a)
{
    // CHECK: hl.lnot
    !a;
}

