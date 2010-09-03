// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <llvm/LLVMContext.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{
namespace
{
    
struct GetValue : boost::static_visitor<llvm::Value *>
{
    llvm::BasicBlock& block;
    llvm::Module& module;
    SymbolTable& symbolTable;

    GetValue(llvm::BasicBlock& block, llvm::Module& module, SymbolTable& symbolTable)
        : block(block), module(module), symbolTable(symbolTable) { }
    
    llvm::Value *operator()(boost::int32_t c)
    {
        return llvm::ConstantInt::get(module.getContext(), llvm::APInt(32, c, true));
    }

    llvm::Value *operator()(const ast::SharedVariable& var)
    {
        return new llvm::LoadInst(symbolTable.get(var->name()), "", &block);
    }
};

}
    
llvm::CallInst *CodeGenerator::genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block, llvm::Module& module)
{
    GetValue getValue(block, module, symbolTable_);
    llvm::Value *arg = fc.apply_visitor(getValue);
    
    return llvm::CallInst::Create(module.getFunction("_rask_print_int"), arg, "", &block);
}
    
    
}
}