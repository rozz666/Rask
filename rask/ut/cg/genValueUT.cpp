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
#include <rask/test/TUTAssert.hpp>
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace tut
{

struct genValue_TestData
{
    llvm::LLVMContext ctx;
    llvm::BasicBlock *block;
    rask::cg::SymbolTable st;
    rask::cg::CodeGenerator cg;
    llvm::AllocaInst *a1;
    
    genValue_TestData()
        : block(llvm::BasicBlock::Create(ctx)), cg(st),
        a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32)))
    {
    }
};

typedef test_group<genValue_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genValue");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::Constant c(10);
    ENSURE(cg.genValue(c, st, *block) == llvm::ConstantInt::get(ctx, llvm::APInt(32, c, true)));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ast::SharedVariable v(new ast::Variable(cst::Identifier::create(Position(), "x")));
    st.add(v->name(), a1);
    
    llvm::Value *val = cg.genValue(v, st, *block);

    ENSURE(llvm::isa<llvm::LoadInst>(val));
    ENSURE(llvm::cast<llvm::LoadInst>(val)->getPointerOperand() == st.get(v->name()));
}

}
