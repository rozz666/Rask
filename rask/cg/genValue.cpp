// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
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
    const SymbolTable& symbolTable;

    GetValue(llvm::BasicBlock& block, const SymbolTable& symbolTable)
        : block(block), symbolTable(symbolTable) { }

    llvm::Value *operator()(const ast::Constant& c)
    {
        return llvm::ConstantInt::get(block.getContext(), llvm::APInt(32, c, true));
    }

    llvm::Value *operator()(const ast::WeakVariable& var)
    {
        return new llvm::LoadInst(symbolTable.get(var.lock()->name()), "", &block);
    }
};

}
    
llvm::Value *CodeGenerator::genValue(const ast::Expression& expr, const SymbolTable& symbolTable, llvm::BasicBlock& block)
{
    GetValue getValue(block, symbolTable);
    return expr.apply_visitor(getValue);
}
    
}
}