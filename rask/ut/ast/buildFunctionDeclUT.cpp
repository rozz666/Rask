// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/VariableFactory.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/ut/ast/VariableFactoryMock.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

struct rask_ast_Builder_buildFunctionDecl : testing::Test
{
    error::SharedLogger logger;
    ast::SharedFunctionTable ft;
    ast::test::SharedVariableFactoryMock variableFactory;
    ast::Builder builder;

    rask_ast_Builder_buildFunctionDecl()
        : logger(new error::Logger), ft(new ast::FunctionTable),
        variableFactory(new ast::test::VariableFactoryMock), builder(logger, ft, variableFactory) { }
};

TEST_F(rask_ast_Builder_buildFunctionDecl, 1)
{
    cst::Function cf;
    cf.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");
    cf.type = cst::Identifier::create(Position(), "void");

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf);

    ASSERT_TRUE(fd);
    ASSERT_EQ(0u, logger->errors().size());

    ast::SharedFunction f = fd->function();
    ASSERT_TRUE(ft->getFunction(f->name().value) == f);
    ASSERT_EQ(cf.name.position, f->name().position);
    ASSERT_EQ(cf.name.value, f->name().value);
}

TEST_F(rask_ast_Builder_buildFunctionDecl, 2)
{
    cst::Function cf1;
    cf1.name = cst::Identifier::create(Position("xxx", 1, 2), "asia");
    cf1.type = cst::Identifier::create(Position(), "void");
    cst::Function cf2;
    cf2.name = cst::Identifier::create(Position("xxx", 10, 2), "asia");
    cf2.type = cst::Identifier::create(Position(), "void");

    builder.buildFunctionDecl(cf1);

    // FIXME: depends on previous call, mock FunctionTable instead
    ASSERT_FALSE(builder.buildFunctionDecl(cf2));
    ASSERT_EQ(2u, logger->errors().size());
    ASSERT_EQ(error::Message::redefinition(cf2.name.position, "asia()"), logger->errors()[0]);
    ASSERT_EQ(error::Message::previousDefinition(cf1.name.position, "asia()"), logger->errors()[1]);
}

TEST_F(rask_ast_Builder_buildFunctionDecl, 3)
{
    cst::Function cf;
    cf.name = cst::Identifier::create(Position(), "asia");
    cf.type = cst::Identifier::create(Position(), "void");
    cf.args.resize(2);
    cf.args[0].name = cst::Identifier::create(Position(), "arg1");
    cf.args[0].type = cst::Identifier::create(Position(), "int32");
    cf.args[1].name = cst::Identifier::create(Position(), "arg2");
    cf.args[1].type = cst::Identifier::create(Position(), "boolean");

    ast::SharedVariable v1 = test::VariableFactory::createShared("x");
    ast::SharedVariable v2 = test::VariableFactory::createShared("y");
    EXPECT_CALL(*variableFactory, createVariable(Ref(cf.args[0].name), ast::INT32))
        .WillOnce(Return(v1));
    EXPECT_CALL(*variableFactory, createVariable(Ref(cf.args[1].name), ast::BOOLEAN))
        .WillOnce(Return(v2));

    boost::optional<ast::FunctionDecl> fd = builder.buildFunctionDecl(cf);

    ASSERT_TRUE(fd);
    ASSERT_EQ(0u, logger->errors().size());

    ast::SharedCustomFunction f = fd->function();
    ASSERT_EQ(2u, f->argCount());
    ASSERT_TRUE(f->arg(0) == v1);
    ASSERT_TRUE(f->arg(1) == v2);
}

TEST_F(rask_ast_Builder_buildFunctionDecl, 4)
{
    cst::Function cf1;
    cf1.name = cst::Identifier::create(Position(), "asia1");
    cf1.type = cst::Identifier::create(Position(), "void");
    cst::Function cf2;
    cf2.name = cst::Identifier::create(Position(), "asia2");
    cf2.type = cst::Identifier::create(Position(), "int32");
    cst::Function cf3;
    cf3.name = cst::Identifier::create(Position(), "asia3");
    cf3.type = cst::Identifier::create(Position(), "boolean");

    boost::optional<ast::FunctionDecl> fd1 = builder.buildFunctionDecl(cf1);
    boost::optional<ast::FunctionDecl> fd2 = builder.buildFunctionDecl(cf2);
    boost::optional<ast::FunctionDecl> fd3 = builder.buildFunctionDecl(cf3);

    ASSERT_TRUE(fd1);
    ASSERT_TRUE(fd2);
    ASSERT_TRUE(fd3);
    ASSERT_EQ(0u, logger->errors().size());

    ASSERT_TRUE(fd1->function()->type() == ast::VOID);
    ASSERT_TRUE(fd2->function()->type() == ast::INT32);
    ASSERT_TRUE(fd3->function()->type() == ast::BOOLEAN);
}

