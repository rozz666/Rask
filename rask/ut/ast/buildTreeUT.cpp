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

    BuilderMock(error::Logger& logger, ast::FunctionTable& ft)
        : ast::Builder(logger, ft), main(test::FunctionFactory().createShared("main"))
    {
        ft.add(main);
    }

    MOCK_METHOD2(buildFunctionDecl, boost::optional<ast::FunctionDecl>(const cst::Function&, ast::VariableFactory& ));
    MOCK_METHOD3(buildFunction, bool(const cst::Function&, ast::SharedCustomFunction, ast::SharedScope));
};

}

struct rask_ast_Builder_buildTree : testing::Test
{
    error::Logger logger;
    ast::FunctionTable ft;
    SharedScopeFactoryMock scopeFactory;
    BuilderMock builder;
    cst::Tree cst;

    rask_ast_Builder_buildTree() : scopeFactory(new ScopeFactoryMock), builder(logger, ft) { }
};

TEST_F(rask_ast_Builder_buildTree, empty)
{
    using namespace rask;

    boost::optional<ast::Tree> ast = builder.buildTree(cst, scopeFactory);

    ASSERT_TRUE(ast);
    ASSERT_EQ(0u, logger.errors().size());
    ASSERT_EQ(0u, ast->functionCount());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctions)
{
    using namespace rask;

    cst.functions.resize(2);

    ast::FunctionDecl fd1(cst::Identifier::create(Position(), "f1"), ast::VOID);
    ast::FunctionDecl fd2(cst::Identifier::create(Position(), "f2"), ast::VOID);
    ast::SharedScope s1(new ast::Scope);
    ast::SharedScope s2(new ast::Scope);

    EXPECT_CALL(*scopeFactory, createScope())
        .WillOnce(Return(s1))
        .WillOnce(Return(s2));
    {
        InSequence s;
        EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[0]), Ref(builder.variableFactory)))
            .WillOnce(Return(fd1));
        EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[1]), _))
            .WillOnce(Return(fd2));
        EXPECT_CALL(builder, buildFunction(Ref(cst.functions[0]), fd1.function(), s1))
            .WillOnce(Return(true));
        EXPECT_CALL(builder, buildFunction(Ref(cst.functions[1]), fd2.function(), s2))
            .WillOnce(Return(true));
    }

    boost::optional<ast::Tree> ast = builder.buildTree(cst, scopeFactory);

    ASSERT_TRUE(ast);
    ASSERT_EQ(0u, logger.errors().size());
    ASSERT_EQ(2u, ast->functionCount());
    ASSERT_TRUE(ast->function(0) == fd1.function());
    ASSERT_TRUE(ast->function(1) == fd2.function());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctionsSecondDeclarationFails)
{
    cst.functions.resize(2);

    EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[0]), _))
        .WillOnce(Return(ast::FunctionDecl(cst::Identifier(), ast::VOID)));
    EXPECT_CALL(builder, buildFunctionDecl(Ref(cst.functions[1]), _))
        .WillOnce(Return(boost::none));

    ASSERT_FALSE(builder.buildTree(cst, scopeFactory));
    ASSERT_EQ(0u, logger.errors().size());
}

TEST_F(rask_ast_Builder_buildTree, twoFunctionsSecondDefinitionFails)
{
    cst.functions.resize(2);

    ast::FunctionDecl fd(cst::Identifier::create(Position(), "f"), ast::VOID);
    ast::SharedScope(new ast::Scope);

    EXPECT_CALL(builder, buildFunctionDecl(_, _))
        .Times(2)
        .WillRepeatedly(Return(fd));
    EXPECT_CALL(builder, buildFunction(_, _, _))
        .Times(2)
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*scopeFactory, createScope())
        .Times(2)
        .WillRepeatedly(Return(ast::SharedScope(new ast::Scope)));

    ASSERT_FALSE(builder.buildTree(cst, scopeFactory));
    ASSERT_EQ(0u, logger.errors().size());
}

TEST_F(rask_ast_Builder_buildTree, noMain)
{
    cst.end = Position("xxx", 1, 2);
    ft = ast::FunctionTable();

    ASSERT_FALSE(builder.buildTree(cst, scopeFactory));
    ASSERT_EQ(1u, logger.errors().size());
    ASSERT_EQ(error::Message::missingMainFunction(Position(cst.end.file)), logger.errors()[0]);
}

