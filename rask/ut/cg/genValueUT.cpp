// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <rask/test/VariableFactory.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>
#include <boost/scoped_ptr.hpp>

using namespace rask;
using namespace testing;

namespace
{

struct CodeGeneratorMock : cg::CodeGenerator
{
    CodeGeneratorMock(cg::SymbolTable& symbolTable) : cg::CodeGenerator(symbolTable, null, null) { }

    MOCK_METHOD2(genFunctionCall, llvm::CallInst *(const ast::FunctionCall&, llvm::BasicBlock&));
};

}

struct rask_cg_CodeGenerator_genValue : testing::Test
{
    test::VariableFactory variableFactory;
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    cg::SymbolTable st;
    CodeGeneratorMock cg;
    llvm::AllocaInst *a1;

    rask_cg_CodeGenerator_genValue()
        : module(new llvm::Module("testModule", ctx)), cg(st),
        a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32)))
    {
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }
};

TEST_F(rask_cg_CodeGenerator_genValue, constant)
{
    ast::Constant c(10);
    ASSERT_TRUE(cg.genValue(c, *block) == llvm::ConstantInt::get(ctx, llvm::APInt(32, c.getInt32(), true)));
}

TEST_F(rask_cg_CodeGenerator_genValue, variable)
{
    ast::SharedVariable v = variableFactory.createShared("x");
    st.add(v->name(), a1);

    llvm::Value *val = cg.genValue(v, *block);

    ASSERT_TRUE(llvm::isa<llvm::LoadInst>(val));
    ASSERT_TRUE(llvm::cast<llvm::LoadInst>(val)->getPointerOperand() == st.get(v->name()));
}

TEST_F(rask_cg_CodeGenerator_genValue, functionCall)
{
    ast::Expression fc = ast::FunctionCall(null, ast::FunctionCall::Arguments(0));

    EXPECT_CALL(cg, genFunctionCall(Ref(getFunctionCall(fc)), Ref(*block)))
        .WillOnce(Return(null));

    llvm::Value *val = cg.genValue(fc, *block);

    ASSERT_TRUE(val == null);
}
