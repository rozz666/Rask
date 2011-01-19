// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/CodeGenerator.hpp>
#include <boost/scoped_ptr.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct CodeGeneratorMock : cg::CodeGenerator
{
    CodeGeneratorMock(cg::SymbolTable& st) : cg::CodeGenerator(st) { }

    MOCK_METHOD2(genValue, llvm::Value *(const ast::Expression&, llvm::BasicBlock&));
};

std::ostream& operator<<(std::ostream& os, const ast::Expression& )
{
    return os << "ast::Expression";
}

}

struct rask_cg_CodeGenerator_genReturn : testing::Test
{
    llvm::LLVMContext ctx;
    cg::SymbolTable st;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    CodeGeneratorMock cg;

    rask_cg_CodeGenerator_genReturn() : module(new llvm::Module("testModule", ctx)), cg(st)
    {
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }
};

TEST_F(rask_cg_CodeGenerator_genReturn, returnExpression)
{
    ast::Return ret(ast::Constant(10));

    llvm::AllocaInst *a1 = new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32));

    EXPECT_CALL(cg, genValue(Ref(ret.expr()), Ref(*block)))
        .WillOnce(Return(a1));

    cg.genReturn(ret, *block);

    ASSERT_EQ(1u, block->size());
    ASSERT_TRUE(llvm::isa<llvm::ReturnInst>(block->front()));
    llvm::ReturnInst *inst = llvm::cast<llvm::ReturnInst>(&block->front());
    ASSERT_TRUE(inst->getReturnValue() == a1);
}
