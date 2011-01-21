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

using namespace rask;
using namespace testing;

struct rask_cg_CodeGenerator_declFunction : testing::Test
{
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module;
    cg::SymbolTable st;
    cg::CodeGenerator cg;
    test::FunctionFactory functionFactory;

    rask_cg_CodeGenerator_declFunction() : module(new llvm::Module("testModule", context)), cg(st) { }
};

TEST_F(rask_cg_CodeGenerator_declFunction, voidFunctionNoArgs)
{
    ast::CustomFunction f = functionFactory.create("f1");

    cg.declFunction(f, *module);

    ASSERT_EQ(1u, module->getFunctionList().size());
    llvm::Function& lf = module->getFunctionList().front();
    ASSERT_EQ(f.name().value, lf.getNameStr());
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    ASSERT_TRUE(llvm::isa<llvm::PointerType>(lf.getType()));
    ASSERT_TRUE(lf.getType()->getElementType() == fType);
    ASSERT_TRUE(lf.getBasicBlockList().empty());
    ASSERT_TRUE(lf.getCallingConv() == llvm::CallingConv::C);
    ASSERT_TRUE(lf.getLinkage() == llvm::Function::ExternalLinkage);
    ASSERT_TRUE(lf.getParent() == module.get());
    ASSERT_TRUE(&lf.getContext() == &context);
}

TEST_F(rask_cg_CodeGenerator_declFunction, voidFunctionTwoArgs)
{
    ast::SharedCustomFunction f = functionFactory.createShared("f2", ast::VOID, 2);

    cg.declFunction(*f, *module);

    ASSERT_EQ(1u, module->getFunctionList().size());
    llvm::Function& lf = module->getFunctionList().front();
    ASSERT_EQ(f->name().value, lf.getNameStr());
    std::vector<const llvm::Type *> fArgs(2, llvm::IntegerType::get(module->getContext(), 32));
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), fArgs, false);
    ASSERT_TRUE(llvm::isa<llvm::PointerType>(lf.getType()));
    ASSERT_TRUE(lf.getType()->getElementType() == fType);
    ASSERT_EQ(2u, lf.arg_size());
    llvm::Function::arg_iterator arg = lf.arg_begin();
    ASSERT_EQ(cg::ARG_PREFIX + f->arg(0)->name().value, arg->getNameStr());
    ++arg;
    ASSERT_EQ(cg::ARG_PREFIX + f->arg(1)->name().value, arg->getNameStr());
    ASSERT_TRUE(lf.getBasicBlockList().empty());
}

TEST_F(rask_cg_CodeGenerator_declFunction, int32FunctionNoArgs)
{
    ast::CustomFunction f = functionFactory.create("f3", ast::INT32);

    cg.declFunction(f, *module);

    ASSERT_EQ(1u, module->getFunctionList().size());
    llvm::Function& lf = module->getFunctionList().front();
    ASSERT_EQ(f.name().value, lf.getNameStr());
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    ASSERT_TRUE(llvm::isa<llvm::PointerType>(lf.getType()));
    ASSERT_TRUE(lf.getType()->getElementType() == fType);
    ASSERT_TRUE(lf.getBasicBlockList().empty());
    ASSERT_TRUE(lf.getCallingConv() == llvm::CallingConv::C);
    ASSERT_TRUE(lf.getLinkage() == llvm::Function::ExternalLinkage);
    ASSERT_TRUE(lf.getParent() == module.get());
    ASSERT_TRUE(&lf.getContext() == &context);
}
