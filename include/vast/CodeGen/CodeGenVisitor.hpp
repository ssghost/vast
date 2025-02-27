// Copyright (c) 2022-present, Trail of Bits, Inc.

#pragma once

#include "vast/CodeGen/CodeGenScope.hpp"
#include "vast/Util/Warnings.hpp"

#include "vast/CodeGen/CodeGenDeclVisitor.hpp"
#include "vast/CodeGen/CodeGenStmtVisitor.hpp"
#include "vast/CodeGen/CodeGenTypeVisitor.hpp"
#include "vast/CodeGen/CodeGenVisitorBase.hpp"
#include "vast/CodeGen/CodeGenFallBackVisitor.hpp"

namespace vast::cg
{
    //
    // DefaultCodeGenVisitorMixin
    //
    // Provides default codegen for statements, declarations, types and comments.
    //
    template< typename Derived >
    struct DefaultCodeGenVisitorMixin
        : CodeGenDeclVisitorMixin< Derived >
        , CodeGenStmtVisitorMixin< Derived >
        , CodeGenTypeVisitorWithDataLayoutMixin< Derived >
    {
        using DeclVisitor = CodeGenDeclVisitorMixin< Derived >;
        using StmtVisitor = CodeGenStmtVisitorMixin< Derived >;
        using TypeVisitor = CodeGenTypeVisitorWithDataLayoutMixin< Derived >;

        using DeclVisitor::Visit;
        using StmtVisitor::Visit;
        using TypeVisitor::Visit;
    };

    //
    // CodeGenVisitor
    //
    // It is paramtetrized by `CodeGenVisitorMixin` that implements all Visit methods.
    // This allows to cofigure Visit implementation, e.g., to provide FallBackVisitor.
    //
    // `MetaGenerator` takes care of attaching location metadata to generated mlir primitives.
    //
    template< typename CGContext, template< typename > class CGVisitorMixin, MetaGeneratorLike MetaGenerator >
    struct CodeGenVisitor
        : CGVisitorMixin< CodeGenVisitor< CGContext, CGVisitorMixin, MetaGenerator > >
        , CodeGenVisitorBaseWithBuilder< CGContext, MetaGenerator >
    {
        using BaseType          = CodeGenVisitorBaseWithBuilder< CGContext, MetaGenerator >;
        using MixinType         = CGVisitorMixin< CodeGenVisitor< CGContext, CGVisitorMixin, MetaGenerator > >;
        using MetaGeneratorType = MetaGenerator;

        CodeGenVisitor(CGContext &ctx, MetaGenerator &gen)
            : BaseType(ctx, gen)
        {}

        using BaseType::set_insertion_point_to_start;
        using BaseType::has_insertion_block;
        using BaseType::clear_insertion_point;

        using MixinType::Visit;
    };

} // namespace vast::cg
