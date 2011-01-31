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
    llvm::BasicBlock& entry;

    StatementVisitor(CodeGenerator& cg, llvm::BasicBlock& entry)
        : cg(cg), entry(entry) { }

    void operator()(const ast::FunctionCall& fc)
    {
        cg.genFunctionCall(fc, entry);
    }

    void operator()(const ast::VariableDecl& vd)
    {
        cg.genVariableDecl(vd, entry);
    }

    void operator()(const ast::Return& ret)
    {
        cg.genReturn(ret, entry);
    }
};

void CodeGenerator::genFunction(const ast::CustomFunction& f, llvm::Module& module)
{
    llvm::Function *func = module.getFunction(f.name().value);
    llvm::BasicBlock *entry = basicBlockFactory_->createBasicBlock(module.getContext(), "entry", func);

    if (f.argCount() > 0)
    {
        llvm::BasicBlock *argsBlock = basicBlockFactory_->createBasicBlockBefore(module.getContext(), "args", func, entry);
        const llvm::IntegerType *type = llvm::IntegerType::get(module.getContext(), 32);

        BOOST_FOREACH(llvm::Argument& arg, func->getArgumentList())
        {
            const cst::Identifier& argName = f.arg(arg.getArgNo())->name();
            llvm::AllocaInst *alloca = instructionFactory_->createAlloca(type, LOCAL_ARG_PREFIX + argName.value, argsBlock);
            instructionFactory_->createStore(&arg, alloca, argsBlock);
            symbolTable_->add(argName, alloca);
        }

        instructionFactory_->createBranch(entry, argsBlock);
    }

    StatementVisitor sv(*this,  *entry);

    for (std::size_t i = 0; i != f.stmtCount(); ++i)
    {
        f.stmt(i).apply_visitor(sv);
    }

    if (f.type() == ast::VOID)
    {
        instructionFactory_->createReturn(module.getContext(), entry);
    }
}

}
}

