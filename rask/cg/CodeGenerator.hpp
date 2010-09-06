// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_CODEGENERATOR_HPP
#define RASK_CG_CODEGENERATOR_HPP

#include <memory>
#include <rask/ast/Function.hpp>
#include <rask/ast/Tree.hpp>
#include <rask/cg/SymbolTable.hpp>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/Instructions.h>

namespace rask
{
namespace cg
{

class CodeGenerator
{
public:

    CodeGenerator(SymbolTable& symbolTable) : symbolTable_(symbolTable) { }
    
    virtual llvm::CallInst *genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block, llvm::Module& module);
    virtual llvm::AllocaInst *genVariableDecl(const ast::VariableDecl& vd, llvm::BasicBlock& block);
    virtual llvm::Function *genFunction(const ast::Function& f, llvm::Module& module);
    virtual void declFunction(const ast::Function& f, llvm::Module& module);
    virtual void declBuiltinFunctions(llvm::Module& module);
    virtual std::auto_ptr<llvm::Module> genModule(const ast::Tree& ast, llvm::LLVMContext& context);

private:

    SymbolTable& symbolTable_;
};
    
}
}

#endif /* RASK_CG_CODEGENERATOR_HPP */