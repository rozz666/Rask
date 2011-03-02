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
#include <llvm/DerivedTypes.h>
#include <rask/test/FunctionFactory.hpp>
#include <rask/null.hpp>
#include <boost/scoped_ptr.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

struct rask_cg_CodeGenerator_genModule : testing::Test
{
};

namespace
{

struct CodeGeneratorMock : cg::CodeGenerator
{
    CodeGeneratorMock() : cg::CodeGenerator(null, null, null, null) { }

    MOCK_METHOD2(genFunction, void(const ast::CustomFunction&, llvm::Module&));
    MOCK_METHOD2(declFunction, void(const ast::CustomFunction&, llvm::Module&));
    MOCK_METHOD1(declBuiltinFunctions, void(llvm::Module&));
};

}

TEST_F(rask_cg_CodeGenerator_genModule, twoFunctions)
{
    llvm::LLVMContext context;
    ast::SharedCustomFunction f1 = test::FunctionFactory::createShared("abc");
    ast::SharedCustomFunction f2 = test::FunctionFactory::createShared("def");
    ast::Tree ast;
    ast.add(f1);
    ast.add(f2);

    CodeGeneratorMock cg;

    boost::scoped_ptr<llvm::Module> module(new llvm::Module("xxx", context));

    {
        InSequence s;
        EXPECT_CALL(cg, declBuiltinFunctions(Ref(*module)));
        EXPECT_CALL(cg, declFunction(Ref(*f1), Ref(*module)));
        EXPECT_CALL(cg, declFunction(Ref(*f2), Ref(*module)));
        EXPECT_CALL(cg, genFunction(Ref(*f1), Ref(*module)));
        EXPECT_CALL(cg, genFunction(Ref(*f2), Ref(*module)));
    }

    cg.genModule(ast, context, *module);
}
