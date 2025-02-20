// Copyright (c) 2022-present, Trail of Bits, Inc.

#ifndef VAST_DIALECT_IR_ABIOPS
#define VAST_DIALECT_IR_ABIOPS

include "mlir/Interfaces/CallInterfaces.td"
include "mlir/Interfaces/CastInterfaces.td"

include "mlir/IR/OpBase.td"
include "mlir/IR/EnumAttr.td"
include "mlir/IR/BuiltinAttributes.td"
include "mlir/IR/FunctionInterfaces.td"

include "vast/Interfaces/SymbolInterface.td"
include "vast/Dialect/HighLevel/HighLevelLinkage.td"

def DirectOp
    : ABI_Op< "direct" >
    , Arguments<(ins Variadic<AnyType>:$value)>
    , Results<(outs Variadic<AnyType>:$result)>
{
    let summary = "Pass value directly - usually means by register";
    let description = [{ Pass value directly - usually means by register. }];
}

def RetDirectOp
    : ABI_Op< "ret_direct" >
    , Arguments<(ins AnyType:$value)>
    , Results<(outs Variadic<AnyType>:$result)>
{
    let summary = "Value is returned directly.";
    let description = [{ Value is returned directly. }];
}

def TodoOp
    : ABI_Op< "todo" >
    , Arguments<(ins AnyType:$value)>
    , Results<(outs Variadic<AnyType>:$result)>
{
    let summary = "Not implement yet.";
    let description = [{ Not implemented yet }];
}


class HasBody< string mnemonic >
    : ABI_Op< mnemonic >
    , Results<(outs Variadic<AnyType>:$results)>
{
    let regions = (region AnyRegion:$body);

    let skipDefaultBuilders = 1;
    let builders = [
        OpBuilder<(ins
            "mlir::TypeRange":$types,
            CArg< "BuilderCallback", "std::nullopt" >:$bodyBuilder
        )>
    ];
}

def PrologueOp : HasBody< "prologue" >
{
    let summary = "WIP";
    let description = [{ WIP }];
}

def EpilogueOp : HasBody< "epilogue" >
{
    let summary = "WIP";
    let description = [{ WIP }];
}

def CallArgsOp : HasBody< "call_args" >
{
    let summary = "Not implement yet.";
    let description = [{ WIP }];
}

def CallRetsOp : HasBody< "call_rets" >
{
    let summary = "Not implement yet.";
    let description = [{ WIP }];
}

def CallExecutionOp
    : ABI_Op< "call_exec", [DeclareOpInterfaceMethods< CallOpInterface >] >
    , Arguments<(ins
        FlatSymbolRefAttr:$callee,
        Variadic<AnyType>:$args)>
    , Results<(outs AnyType:$result)>
{
    let summary = "WIP";
    let description = [{ WIP }];

    let regions = (region AnyRegion:$body);

    let skipDefaultBuilders = 1;
    let builders = [
        OpBuilder< (ins "llvm::StringRef":$callee,
                        "mlir::TypeRange":$results,
                        CArg<"mlir::ValueRange", "{}">:$operands,
                        CArg< "BuilderCallback", "std::nullopt" >:$bodyBuilder)
                 >
  ];
}

def YieldOp
    : ABI_Op< "yield", [Terminator] >
    , Arguments<(ins Variadic<AnyType>:$values)>
    , Results<(outs Variadic<AnyType>:$result)>
{
    let summary = "WIP";
    let description = [{ WIP }];
}


def CallOp
  : ABI_Op< "call", [DeclareOpInterfaceMethods<CallOpInterface>] >
  , Arguments<(ins
      FlatSymbolRefAttr:$callee,
      Variadic<AnyType>:$args)>
  , Results<(outs Variadic<AnyType>:$results)>
{
  let summary = "ABI call operation";
  let description = [{ ABI call operation }];

  let skipDefaultBuilders = 1;
  let builders = [
    OpBuilder< (ins "llvm::StringRef":$callee,
                "mlir::TypeRange":$results,
                CArg<"mlir::ValueRange", "{}">:$operands),
    [{
      $_state.addOperands(operands);
      $_state.addAttribute("callee", mlir::SymbolRefAttr::get($_builder.getContext(), callee));
      $_state.addTypes(results);
    }]>];
}

// TODO(abi): This is mostly copy & paste of hl.func. Once ABI dialect is stable,
//            extract shared parts from both definitions into some helper.
class ABIFuncBase< string mnemonic >
    : ABI_Op< mnemonic, [ AutomaticAllocationScope, CallableOpInterface, FunctionOpInterface,
                          IsolatedFromAbove, Symbol, NoTerminator ] >
{
    let description = [{
        Placeholder.
    }];

    let arguments =(ins
        SymbolNameAttr:$sym_name,
        TypeAttrOf<FunctionType>:$function_type,
        DefaultValuedAttr<GlobalLinkageKind, "hl::GlobalLinkageKind::ExternalLinkage">:$linkage,
        OptionalAttr<StrAttr>:$sym_visibility,
        OptionalAttr<DictArrayAttr>:$arg_attrs,
        OptionalAttr<DictArrayAttr>:$res_attrs
    );

    let regions = (region AnyRegion:$body);
    let skipDefaultBuilders = 1;

    let builders = [OpBuilder<(ins
        "llvm::StringRef":$name,
        "mlir::FunctionType":$type,
        CArg< "hl::GlobalLinkageKind", "hl::GlobalLinkageKind::ExternalLinkage" >:$linkage,
        CArg< "llvm::ArrayRef<mlir::NamedAttribute>", "{}" >:$attrs,
        CArg< "llvm::ArrayRef<mlir::DictionaryAttr>", "{}" >:$arg_attrs,
        CArg< "llvm::ArrayRef<mlir::DictionaryAttr>", "{}" >:$res_attrs), [{
            Builder::InsertionGuard guard($_builder);

            $_state.addRegion();
            $_state.addAttribute(
                mlir::SymbolTable::getSymbolAttrName(), $_builder.getStringAttr(name)
            );
            $_state.addAttribute(getFunctionTypeAttrName($_state.name),
                                 mlir::TypeAttr::get(type));
            $_state.addAttribute(
                "linkage", hl::GlobalLinkageKindAttr::get($_builder.getContext(), linkage)
            );
            $_state.attributes.append(attrs.begin(), attrs.end());

            if (arg_attrs.empty())
                return;

            mlir::function_interface_impl::addArgAndResultAttrs(
                $_builder, $_state, arg_attrs, res_attrs,
                getArgAttrsAttrName($_state.name), getResAttrsAttrName($_state.name)
            );
        }] >
    ];

    let extraClassDeclaration = [{
        //===------------------------------------------------------------------===//
        // CallableOpInterface
        //===------------------------------------------------------------------===//

        /// Returns the region on the current operation that is callable. This may
        /// return null in the case of an external callable object, e.g. an external
        /// function.
        ::mlir::Region *getCallableRegion() {
            return isExternal() ? nullptr : &getBody();
        }

        /// Returns the results types that the callable region produces when
        /// executed.
        llvm::ArrayRef<Type> getCallableResults() {
            return getFunctionType().getResults();
        }

        //===------------------------------------------------------------------===//
        // FunctionOpInterface Methods
        //===------------------------------------------------------------------===//
        /// Returns the argument types of this function.
        llvm::ArrayRef<Type> getArgumentTypes() { return getFunctionType().getInputs(); }

        /// Returns the result types of this function.
        llvm::ArrayRef<Type> getResultTypes() { return getFunctionType().getResults(); }

        //===------------------------------------------------------------------===//
        // SymbolOpInterface Methods
        //===------------------------------------------------------------------===//

        bool isDeclaration() { return isExternal(); }
    }];
}

def FuncOp : ABIFuncBase< "func" >
{
    let summary = [{ Function with transformed type. }];
}

def WrapFuncOp : ABIFuncBase< "wrap_fn" >
{
    let summary = [{ Function that defines abi transformation of args. }];
}

#endif // VAST_DIALECT_IR_ABIOPS
