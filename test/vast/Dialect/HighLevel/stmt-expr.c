// RUN: vast-cc --ccopts -xc --from-source %s | FileCheck %s
// RUN: vast-cc --ccopts -xc --from-source %s > %t && vast-opt %t | diff -B %t -

int main() {
    // CHECK: hl.stmt.expr : !hl.int
    // CHECK:   hl.var "x"
    // CHECK:   hl.value.yield
    // CHECK:   hl.value.yield
    int v = ({int x = 4; x;});
}
