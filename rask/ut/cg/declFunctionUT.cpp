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
#include <llvm/DerivedTypes.h>
#include <rask/cg/Prefixes.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <gtest/gtest.h>
#include <rask/null.hpp>

using namespace rask;
using namespace testing;

struct rask_cg_CodeGenerator_declFunction : testing::Test
{
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module;
    cg::CodeGenerator cg;
    ast::SharedCustomFunction f;
    llvm::Function *lf;

    rask_cg_CodeGenerator_declFunction() : module(new llvm::Module("testModule", context)), cg(null, null, null, null) { }

    void assertFunction()
    {
        cg.declFunction(*f, *module);

        ASSERT_EQ(1u, module->getFunctionList().size());
        lf = &module->getFunctionList().front();
        ASSERT_EQ(f->name().value, lf->getNameStr());
    }

    void assertFunctionType(llvm::FunctionType *type)
    {
        ASSERT_TRUE(llvm::isa<llvm::PointerType>(lf->getType()));
        ASSERT_TRUE(lf->getType()->getElementType() == type);
    }
};

TEST_F(rask_cg_CodeGenerator_declFunction, voidFunctionNoArgs)
{
    f = test::FunctionFactory::createShared("f1");

    ASSERT_NO_FATAL_FAILURE(assertFunction());

    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    ASSERT_NO_FATAL_FAILURE(assertFunctionType(fType));
    ASSERT_TRUE(lf->getBasicBlockList().empty());
    ASSERT_TRUE(lf->getCallingConv() == llvm::CallingConv::C);
    ASSERT_TRUE(lf->getLinkage() == llvm::Function::ExternalLinkage);
}

TEST_F(rask_cg_CodeGenerator_declFunction, voidFunctionTwoArgs)
{
    f = test::FunctionFactory::createShared("f2", ast::VOID, 2);

    ASSERT_NO_FATAL_FAILURE(assertFunction());

    std::vector<const llvm::Type *> fArgs(2, llvm::IntegerType::get(module->getContext(), 32));
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), fArgs, false);
    ASSERT_NO_FATAL_FAILURE(assertFunctionType(fType));
    ASSERT_EQ(2u, lf->arg_size());
    llvm::Function::arg_iterator arg = lf->arg_begin();
    ASSERT_EQ(cg::ARG_PREFIX + f->arg(0)->name().value, arg->getNameStr());
    ++arg;
    ASSERT_EQ(cg::ARG_PREFIX + f->arg(1)->name().value, arg->getNameStr());
}

TEST_F(rask_cg_CodeGenerator_declFunction, int32FunctionNoArgs)
{
    f = test::FunctionFactory::createShared("f3", ast::INT32);

    ASSERT_NO_FATAL_FAILURE(assertFunction());

    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    assertFunctionType(fType);
}
