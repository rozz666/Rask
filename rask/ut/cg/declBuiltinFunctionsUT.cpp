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
#include <gmock/gmock.h>
#include <rask/null.hpp>

using namespace rask;
using namespace testing;

struct rask_cg_CodeGenerator_declBuiltinFunctions : testing::Test
{
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module;
    cg::SymbolTable st;

    rask_cg_CodeGenerator_declBuiltinFunctions() : module(new llvm::Module("testModule", context))
    {
        cg::CodeGenerator(st, null, null).declBuiltinFunctions(*module);
    }

    void assertFunction(const std::string& name, llvm::FunctionType *functionType)
    {
        llvm::Function *f = module->getFunction(name);

        ASSERT_TRUE(f);
        ASSERT_TRUE(llvm::isa<llvm::PointerType>(f->getType()));
        ASSERT_TRUE(f->getType()->getElementType() == functionType);
        ASSERT_TRUE(f->getBasicBlockList().empty());
        ASSERT_TRUE(f->getLinkage() == llvm::Function::ExternalLinkage);
        ASSERT_TRUE(f->getCallingConv() == llvm::CallingConv::C);
    }
};

TEST_F(rask_cg_CodeGenerator_declBuiltinFunctions, print)
{
    std::vector<const llvm::Type*> printIntArgs;
    printIntArgs.push_back(llvm::IntegerType::get(context, 32));
    llvm::FunctionType *printIntType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), printIntArgs, false);
    assertFunction("print", printIntType);
}

TEST_F(rask_cg_CodeGenerator_declBuiltinFunctions, getInt32)
{
    llvm::FunctionType *getInt32Type = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    assertFunction("getInt32", getInt32Type);
}
