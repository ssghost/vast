// Copyright (c) 2021-present, Trail of Bits, Inc.

#pragma once

#include "vast/Util/Warnings.hpp"

VAST_RELAX_WARNINGS
#include <clang/AST/GlobalDecl.h>
VAST_UNRELAX_WARNINGS

#include "vast/Dialect/HighLevel/HighLevelAttributes.hpp"

namespace vast::hl {

    mlir::SymbolTable::Visibility get_visibility_from_linkage(GlobalLinkageKind linkage);

    GlobalLinkageKind get_declarator_linkage(
        const clang::DeclaratorDecl *decl, clang::GVALinkage linkage, bool is_constant
    );

    GlobalLinkageKind get_function_linkage(clang::GlobalDecl glob);

} // namespace vast::hl
