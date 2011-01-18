// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/test/FunctionFactory.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct ScopeFactoryMock : ast::ScopeFactory
{
    MOCK_METHOD0(createScope, ast::SharedScope());
};

typedef boost::shared_ptr<ScopeFactoryMock> SharedScopeFactoryMock;

struct BuilderMock : ast::Builder
{
    ast::SharedCustomFunction main;

    BuilderMock(error::SharedLogger logger, ast::SharedFunctionTable ft)
        : ast::Builder(logger, ft, null), main(test::FunctionFactory().createShared("main"))
    {
        ft->add(main);
    }

    MOCK_METHOD1(buildFunctionDecl, boost::optional<ast::FunctionDecl>(const cst::Function&));
    MOCK_METHOD3(buildFunction, bool(const cst::Function&, ast::SharedCustomFunction, ast::SharedScope));
};

}

struct rask_ast_Builder_buildTree : testing::Test
{
    error::SharedLogger logger;
    ast::SharedFunctionTable ft;
    SharedScopeFactoryMock scopeFactory;
    BuilderMock builder;
    cst::Tree cst;
    boost::optional<ast::Tree> ast;

    rask_ast_Builder_buildTree()
        : logger(new error::Logger), ft(new ast::FunctionTable), scopeFactory(new ScopeFactoryMock),
        builder(logger, ft) { }

    void assertBuildTree()
    {
        ast = builder.buildTree(cst, scopeFactory);
        ASSERT_TRUE(ast);
        ASSERT_EQ(0u, logger->errors().size());
    }

    void assertBuildTreeFails(unsigned numErrors = 0)
    {
        ASSERT_FALSE(builder.buildTree(cst, scopeFactory));
        ASSERT_EQ(numErrors, logger->errors().size());
    }
};

TEST_F(rask_ast_Builder_buildTree, empty)
{
    ASSERT_NO_FATAL_FAILURE(assertBuildTree());
    ASSERT_EQ(0u, ast->functionCount());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctions)
{
    cst.functions.resize(2);

    ast::FunctionDecl decl1(cst::Identifier::create(Position(), "f1"), ast::VOID);
    ast::FunctionDecl decl2(cst::Identifier::create(Position(), "f2"), ast::VOID);
    ast::SharedScope scope1(new ast::Scope);
    ast::SharedScope scope2(new ast::Scope);

    EXPECT_CALL(*scopeFactory, createScope())
        .WillOnce(Return(scope1))
        .WillOnce(Return(scope2));
    {
        InSequence s;
        EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[0])))
            .WillOnce(Return(decl1));
        EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[1])))
            .WillOnce(Return(decl2));
        EXPECT_CALL(builder, buildFunction(Ref(cst.functions[0]), decl1.function(), scope1))
            .WillOnce(Return(true));
        EXPECT_CALL(builder, buildFunction(Ref(cst.functions[1]), decl2.function(), scope2))
            .WillOnce(Return(true));
    }

    ASSERT_NO_FATAL_FAILURE(assertBuildTree());
    ASSERT_EQ(2u, ast->functionCount());
    ASSERT_TRUE(ast->function(0) == decl1.function());
    ASSERT_TRUE(ast->function(1) == decl2.function());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctionsSecondDeclarationFails)
{
    cst.functions.resize(2);

    EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[0])))
        .WillOnce(Return(ast::FunctionDecl(cst::Identifier(), ast::VOID)));
    EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[1])))
        .WillOnce(Return(boost::none));

    ASSERT_NO_FATAL_FAILURE(assertBuildTreeFails());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctionsSecondDefinitionFails)
{
    cst.functions.resize(2);

    ast::FunctionDecl fd(cst::Identifier::create(Position(), "f"), ast::VOID);
    ast::SharedScope(new ast::Scope);

    EXPECT_CALL(builder, buildFunctionDecl(_))
        .Times(2)
        .WillRepeatedly(Return(fd));
    EXPECT_CALL(builder, buildFunction(_, _, _))
        .Times(2)
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*scopeFactory, createScope())
        .Times(2)
        .WillRepeatedly(Return(ast::SharedScope(new ast::Scope)));
    ASSERT_NO_FATAL_FAILURE(assertBuildTreeFails());
}

TEST_F(rask_ast_Builder_buildTree, noMain)
{
    cst.end = Position("xxx", 1, 2);
    *ft = ast::FunctionTable();
    ASSERT_NO_FATAL_FAILURE(assertBuildTreeFails(1));
    ASSERT_EQ(error::Message::missingMainFunction(Position(cst.end.file)), logger->errors()[0]);
}

