// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/TypeFactory.hpp>
#include <llvm/LLVMContext.h>
#include <gmock/gmock.h>
#include <llvm/DerivedTypes.h>

using namespace rask;
using namespace testing;

struct rask_cg_TypeFactory : testing::Test
{
    llvm::LLVMContext context;
    cg::TypeFactory typeFactory;
};

TEST_F(rask_cg_TypeFactory, voidType)
{
    ASSERT_TRUE(llvm::Type::getVoidTy(context) == typeFactory.getType(context, ast::VOID));
}

TEST_F(rask_cg_TypeFactory, int32Type)
{
    ASSERT_TRUE(llvm::IntegerType::get(context, 32) == typeFactory.getType(context, ast::INT32));
}

TEST_F(rask_cg_TypeFactory, booleanType)
{
    ASSERT_TRUE(llvm::IntegerType::get(context, 1) == typeFactory.getType(context, ast::BOOLEAN));
}

