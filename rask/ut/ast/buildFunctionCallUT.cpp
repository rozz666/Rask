// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/ast/BuiltinFunction.hpp>
#include <rask/ut/ast/ScopeMockNew.hpp>
#include <rask/null.hpp>
#include <rask/ut/cst/OperatorStubs.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BuilderMock : ast::Builder
{
    BuilderMock(error::SharedLogger logger, ast::SharedFunctionTable ft)
        : ast::Builder(logger, ft, null) { }

    MOCK_METHOD2(buildExpression, boost::optional<ast::Expression>(const cst::Expression&, ast::SharedScope));
};

}

struct rask_ast_Builder_buildFunctionCall : testing::Test
{
    error::SharedLogger logger;
    ast::SharedFunctionTable ft;
    BuilderMock builder;
    std::string file;
    cst::FunctionCall ccall;
    boost::optional<ast::FunctionCall> call;
    ast::SharedBuiltinFunction f;

    rask_ast_Builder_buildFunctionCall()
        : logger(new error::Logger), ft(new ast::FunctionTable),
        builder(logger, ft), file("test.rask") { }

    void assertFailureWithError(const error::Message& msg)
    {
        ASSERT_FALSE(builder.buildFunctionCall(ccall, null));
        ASSERT_EQ(1u, logger->errors().size());
        ASSERT_EQ(msg, logger->errors()[0]);
    }

    void assertSuccessWithArgs(unsigned numArgs)
    {
        ASSERT_TRUE(call);
        ASSERT_EQ(0u, logger->errors().size());
        ASSERT_TRUE(call->function().lock() == f);
        ASSERT_EQ(numArgs, call->args().size());
    }
};

TEST_F(rask_ast_Builder_buildFunctionCall, functionWithNoArguments)
{
    f.reset(new ast::BuiltinFunction("f", ast::VOID, 0));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    call = builder.buildFunctionCall(ccall, null);
    assertSuccessWithArgs(0);
}

TEST_F(rask_ast_Builder_buildFunctionCall, functionWithTwoArguments)
{
    f.reset(new ast::BuiltinFunction("f", ast::VOID, 2));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    ccall.args.resize(2);
    ast::Constant dummy1(1);
    ast::Constant dummy2(2);
    ast::SharedScope scope(new ast::ScopeMock);

    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[0]), scope))
        .WillOnce(Return(ast::Expression(dummy1)));
    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[1]), scope))
        .WillOnce(Return(ast::Expression(dummy2)));

    call = builder.buildFunctionCall(ccall, scope);
    ASSERT_NO_FATAL_FAILURE(assertSuccessWithArgs(2));
    ASSERT_TRUE(getConstant(call->args()[0]) == dummy1);
    ASSERT_TRUE(getConstant(call->args()[1]) == dummy2);
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownIdentifier)
{
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "xxx");
    ccall.args.resize(1);

    assertFailureWithError(error::Message::unknownIdentifier(ccall.function.position, ccall.function.value));
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownFunction)
{
    f.reset(new ast::BuiltinFunction("abc", ast::VOID, 1));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    assertFailureWithError(error::Message::functionNotFound(ccall.function.position, "abc()"));
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownFunctionWithTwoArguments)
{
    f.reset(new ast::BuiltinFunction("print", ast::VOID, 1));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "print");
    ccall.args.resize(2);

    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[0]), _))
        .WillOnce(Return(null));
    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[1]), _))
        .WillOnce(Return(null));

    assertFailureWithError(error::Message::functionNotFound(ccall.function.position, "print(int32, int32)"));
}
