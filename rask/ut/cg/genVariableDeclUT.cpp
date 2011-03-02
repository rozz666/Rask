// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/Prefixes.hpp>
#include <rask/test/VariableFactory.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/BasicBlock.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <gmock/gmock.h>
#include <rask/null.hpp>

using namespace rask;
using namespace testing;

namespace
{

struct CodeGeneratorMock : rask::cg::CodeGenerator
{
    CodeGeneratorMock(rask::cg::SharedSymbolTable st) : rask::cg::CodeGenerator(st, null, null, null) { }

    MOCK_METHOD2(genValue, llvm::Value *(const ast::Expression& expr, llvm::BasicBlock& block));
};

std::ostream& operator<<(std::ostream& os, const ast::Expression& expr)
{
    return os << "ast::Expression";
}

}

struct rask_cg_CodeGenerator_genVariableDecl : testing::Test
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::BasicBlock> block;
    cg::SharedSymbolTable st;
    CodeGeneratorMock cg;

    rask_cg_CodeGenerator_genVariableDecl()
        : block(llvm::BasicBlock::Create(ctx)), st(new cg::SymbolTable), cg(st) { }
};


TEST_F(rask_cg_CodeGenerator_genVariableDecl, generateDeclaration)
{
    cst::Identifier name;
    name.value = "asia";
    ast::VariableDecl vd(test::VariableFactory::createShared(name), ast::Constant(10));

    llvm::Value *value = llvm::ConstantInt::get(ctx, llvm::APInt(32, 10, true));
    EXPECT_CALL(cg, genValue(Ref(vd.value()), Ref(*block))).WillOnce(Return(value));

    llvm::AllocaInst *alloc = cg.genVariableDecl(vd, *block);

    ASSERT_EQ(2u, block->size());
    llvm::BasicBlock::iterator it = block->begin();

    ASSERT_TRUE(&*it == alloc);
    ASSERT_EQ(cg::LOCAL_VAR_PREFIX + name.value, alloc->getNameStr());
    ASSERT_TRUE(alloc->getAllocatedType() == llvm::IntegerType::get(ctx, 32));
    ASSERT_TRUE(st->get(name) == alloc);
    ++it;

    ASSERT_TRUE(llvm::isa<llvm::StoreInst>(*it));
    llvm::StoreInst *store = llvm::cast<llvm::StoreInst>(&*it);
    ASSERT_TRUE(store->getValueOperand() == value);
    ASSERT_TRUE(store->getPointerOperand() == alloc);
}
