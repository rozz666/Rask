// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

struct StatementVisitor : boost::static_visitor<void>
{
    CodeGenerator& cg;
    llvm::Module& module;
    llvm::BasicBlock& entry;
    
    StatementVisitor(CodeGenerator& cg, llvm::Module& module, llvm::BasicBlock& entry) : cg(cg), module(module), entry(entry) { }

    void operator()(const ast::FunctionCall& fc)
    {
        cg.genFunctionCall(fc, entry, module);
    }

    void operator()(const ast::VariableDecl& vd)
    {
        cg.genVariableDecl(vd, entry);
    }
};
        
llvm::Function *CodeGenerator::genFunction(const ast::Function& f, llvm::Module& module)
{
    llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(module.getContext()), false);
    llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &module);

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(module.getContext(), "entry", func);

    StatementVisitor sv(*this, module, *entry);
    
    for (std::size_t i = 0; i != f.stmtCount(); ++i)
    {
        f.stmt(i).apply_visitor(sv);
    }

    llvm::ReturnInst::Create(module.getContext(), entry);
    
    return func;
}
    
}
}

