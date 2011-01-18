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
    const std::string file;
    ast::SharedFunctionTable ft;
    BuilderMock builder;
    cst::FunctionCall ccall;
    ast::Constant dummy1;
    ast::Constant dummy2;
    ast::SharedScope scope;

    rask_ast_Builder_buildFunctionCall()
        : logger(new error::Logger), file("test.rask"), ft(new ast::FunctionTable),
        builder(logger, ft), dummy1(1), dummy2(2),
        scope(new ast::ScopeMock) { }
};

TEST_F(rask_ast_Builder_buildFunctionCall, functionWithNoArguments)
{
    ast::SharedBuiltinFunction f(new ast::BuiltinFunction("f", ast::VOID, 0));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall, scope);

    ASSERT_TRUE(call);
    ASSERT_EQ(0u, logger->errors().size());
    ASSERT_TRUE(call->function().lock() == f);
    ASSERT_EQ(0u, call->args().size());
}

TEST_F(rask_ast_Builder_buildFunctionCall, functionWithTwoArguments)
{
    ast::SharedBuiltinFunction f(new ast::BuiltinFunction("f", ast::VOID, 2));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    ccall.args.resize(2);

    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[0]), scope))
        .WillOnce(Return(ast::Expression(dummy1)));
    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[1]), scope))
        .WillOnce(Return(ast::Expression(dummy2)));

    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall, scope);

    ASSERT_TRUE(call);
    ASSERT_EQ(0u, logger->errors().size());
    ASSERT_TRUE(call->function().lock() == f);
    ASSERT_EQ(2u, call->args().size());
    ASSERT_TRUE(getConstant(call->args()[0]) == dummy1);
    ASSERT_TRUE(getConstant(call->args()[1]) == dummy2);
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownIdentifier)
{
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "xxx");
    ccall.args.resize(1);

    ASSERT_FALSE(builder.buildFunctionCall(ccall, scope));
    ASSERT_EQ(1u, logger->errors().size());
    ASSERT_EQ(error::Message::unknownIdentifier(ccall.function.position, ccall.function.value), logger->errors()[0]);
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownFunction)
{
    ast::SharedBuiltinFunction f(new ast::BuiltinFunction("abc", ast::VOID, 1));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);

    ASSERT_FALSE(builder.buildFunctionCall(ccall, scope));
    ASSERT_EQ(1u, logger->errors().size());
    ASSERT_EQ(error::Message::functionNotFound(ccall.function.position, "abc()"), logger->errors()[0]);
}

TEST_F(rask_ast_Builder_buildFunctionCall, unknownFunctionWithTwoArguments)
{
    ast::SharedBuiltinFunction f(new ast::BuiltinFunction("print", ast::VOID, 1));
    ft->add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "print");
    ccall.args.resize(2);

    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[0]), _))
        .WillOnce(Return(null));
    EXPECT_CALL(builder, buildExpression(Ref(ccall.args[1]), _))
        .WillOnce(Return(null));

    ASSERT_FALSE(builder.buildFunctionCall(ccall, scope));
    ASSERT_EQ(1u, logger->errors().size());
    ASSERT_EQ(error::Message::functionNotFound(ccall.function.position, "print(int32, int32)"), logger->errors()[0]);
}
