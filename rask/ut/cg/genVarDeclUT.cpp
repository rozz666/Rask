// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/BasicBlock.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>

namespace tut
{

struct genVarDecl_TestData
{
    llvm::LLVMContext ctx;
    std::auto_ptr<llvm::BasicBlock> block;
    rask::cg::CodeGenerator cg;
    
    genVarDecl_TestData() : block(llvm::BasicBlock::Create(ctx))
    {
    }
};

typedef test_group<genVarDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genVarDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Identifier name;
    name.value = "asia";
    ast::VarDecl vd(name, 10);

    llvm::AllocaInst *alloc = cg.genVarDecl(vd, *block);

    ensure_equals("2 instr", block->size(), 2u);
    llvm::BasicBlock::iterator it = block->begin();

    ensure("alloca", &*it == alloc);
    ensure_equals("alloca name", alloc->getNameStr(), name.value);
    ensure("alloca type", alloc->getAllocatedType() == llvm::IntegerType::get(ctx, 32));
    ++it;

    ensure("store", llvm::isa<llvm::StoreInst>(*it));
    llvm::StoreInst *store = llvm::cast<llvm::StoreInst>(&*it);
    ensure("store value", store->getOperand(0) == llvm::ConstantInt::get(ctx, llvm::APInt(32, vd.value(), true)));
    ensure("store ptr", store->getOperand(1) == alloc);
}


}
