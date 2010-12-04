// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/foreach.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/Prefixes.hpp>

namespace rask
{
namespace cg
{

struct StatementVisitor : boost::static_visitor<void>
{
    CodeGenerator& cg;
    llvm::Module& module;
    llvm::BasicBlock& entry;
    SymbolTable& st;

    StatementVisitor(CodeGenerator& cg, llvm::Module& module, llvm::BasicBlock& entry, SymbolTable& st)
        : cg(cg), module(module), entry(entry), st(st) { }

    void operator()(const ast::FunctionCall& fc)
    {
        cg.genFunctionCall(fc, entry, module);
    }

    void operator()(const ast::VariableDecl& vd)
    {
        cg.genVariableDecl(vd, entry);
    }

    void operator()(const ast::Return& ret)
    {
        cg.genReturn(ret, entry, st, module);
    }
};
        
void CodeGenerator::genFunction(const ast::CustomFunction& f, llvm::Module& module)
{
    llvm::Function *func = module.getFunction(f.name().value);
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(module.getContext(), "entry", func);

    if (f.argCount() > 0)
    {
        llvm::BasicBlock *argsBlock = llvm::BasicBlock::Create(module.getContext(), "args", func, entry);
        const llvm::IntegerType *type = llvm::IntegerType::get(module.getContext(), 32);
        
        BOOST_FOREACH(llvm::Argument& arg, func->getArgumentList())
        {
            const cst::Identifier& argName = f.arg(arg.getArgNo())->name();
            llvm::AllocaInst *alloca = new llvm::AllocaInst(type, LOCAL_ARG_PREFIX + argName.value, argsBlock);
            new llvm::StoreInst(&arg, alloca, argsBlock);
            symbolTable_.add(argName, alloca);
        }

        llvm::BranchInst::Create(entry, argsBlock);
    }

    StatementVisitor sv(*this, module, *entry, symbolTable_);
    
    for (std::size_t i = 0; i != f.stmtCount(); ++i)
    {
        f.stmt(i).apply_visitor(sv);
    }

    if (f.type() == ast::VOID)
    {
        llvm::ReturnInst::Create(module.getContext(), entry);
    }
}

}
}

