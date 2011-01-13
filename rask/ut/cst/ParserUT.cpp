// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cst/Parser.hpp>
#include <gtest/gtest.h>
#include <algorithm>

#define ASSERT_IDENTIFIER(expectedValue, expectedPosition, actual) \
    ASSERT_EQ(expectedValue, (actual).value); \
    ASSERT_EQ(expectedPosition, (actual).position)

#define ASSERT_CONST(expectedValue, expectedPosition, actual) \
    ASSERT_EQ(expectedValue, getConstant(actual).value); \
    ASSERT_EQ(expectedPosition, getConstant(actual).position)

#define ASSERT_VARIABLE(expectedName, expectedPosition, actual) \
    ASSERT_IDENTIFIER(expectedName, expectedPosition, getIdentifier(actual))

#define ASSERT_OPERATOR(expectedTag, expectedPosition, actual) \
    ASSERT_TRUE(expectedTag == (actual).op.tag); \
    ASSERT_EQ(expectedPosition, (actual).op.position)

#define ASSERT_NO_ERRORS() \
    ASSERT_EQ(0u, errorLogger.errors().size())

struct rask_cst_Parser : testing::Test
{
    std::stringstream source;
    rask::error::Logger errorLogger;
    std::string file;
    rask::cst::Parser parser;

    rask_cst_Parser()
        : file("test.rask"), parser(errorLogger) { }

    void ensureError(const rask::error::Message& msg)
    {
        ASSERT_EQ(1u, errorLogger.errors().size());
        ASSERT_EQ(msg, errorLogger.errors()[0]);
    }

    rask::Position at(unsigned row, unsigned line)
    {
        return rask::Position(file, row, line);
    }

    boost::optional<rask::cst::Tree> parseFile()
    {
        rask::InputStream sourceStream(file, source);
        return parser.parseFile(sourceStream);
    }
};

TEST_F(rask_cst_Parser, onlyMain)
{
    using namespace rask;

    source << "main() -> void\n{\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_EQ(1u, tree->functions.size());
    ASSERT_IDENTIFIER("main", at(1, 1), tree->functions[0].name);
    ASSERT_NO_ERRORS();
}

TEST_F(rask_cst_Parser, emptySource)
{
    using namespace rask;

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingMainFunction(Position(file)));
}

TEST_F(rask_cst_Parser, noReturnTypeAfterMain)
{
    using namespace rask;

    source << "main() -> \n{\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingReturnType(at(2, 1)));
}

TEST_F(rask_cst_Parser, noOpeningBraceAfterMain)
{
    using namespace rask;

    source << "main() -> void\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingOpeningBrace(at(2, 1)));
}

TEST_F(rask_cst_Parser, noClosingBraceAfterMain)
{
    using namespace rask;

    source << "main() -> void\n{\n";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingClosingBrace(at(3, 1)));
}

TEST_F(rask_cst_Parser, noOpeningParenInMain)
{
    using namespace rask;

    source << "main) -> void\n{\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingOpeningParen(at(1, 5)));
}

TEST_F(rask_cst_Parser, noClosingParenInMain)
{
    using namespace rask;

    source << "main( -> void\n{\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingClosingParen(at(1, 7)));
}

TEST_F(rask_cst_Parser, noGtInRightArrowInMain)
{
    using namespace rask;

    source << "main() - void\n{\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingRightArrow(at(1, 8)));
}

TEST_F(rask_cst_Parser, noHyphenInRightArrowInMain)
{
    using namespace rask;

    source << "main() > void\n{\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingRightArrow(at(1, 8)));
}

TEST_F(rask_cst_Parser, functionCallsInMain)
{
    using namespace rask;

    source << "main() -> void\n{\n    abcd();\n    efgh(-2);\n    ijkl(2, 3);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());
    cst::Function& f = tree->functions[0];
    ASSERT_IDENTIFIER("main", at(1, 1), f.name);
    ASSERT_EQ(3u, f.stmts.size());

    cst::FunctionCall& fc1 = getFunctionCall(f.stmts[0]);
    ASSERT_IDENTIFIER("abcd", at(3, 5), fc1.function);
    ASSERT_EQ(0u, fc1.args.size());

    cst::FunctionCall& fc2 = getFunctionCall(f.stmts[1]);
    ASSERT_IDENTIFIER("efgh", at(4, 5), fc2.function);
    ASSERT_EQ(1u, fc2.args.size());
    ASSERT_CONST(-2, at(4, 10), fc2.args[0]);

    cst::FunctionCall& fc3 = getFunctionCall(f.stmts[2]);
    ASSERT_IDENTIFIER("ijkl", at(5, 5), fc3.function);
    ASSERT_EQ(2u, fc3.args.size());
    ASSERT_CONST(2, at(5, 10), fc3.args[0]);
    ASSERT_CONST(3, at(5, 13), fc3.args[1]);
}

TEST_F(rask_cst_Parser, noSemicolonAfternFunctionCall)
{
    using namespace rask;

    source << "main() -> void\n{\n    print(1)\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingSemicolon(at(4, 1)));
}

TEST_F(rask_cst_Parser, noOpeningParenAfterFunctionCall)
{
    using namespace rask;

    source << "main() -> void\n{\n    print;\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingOpeningParen(at(3, 10)));
}

TEST_F(rask_cst_Parser, noClosingParenAfterFunctionCall)
{
    using namespace rask;

    source << "main() -> void\n{\n    print(5 5;\n}";

    ASSERT_TRUE(!parseFile());
    ensureError(error::Message::missingClosingParen(at(3, 13)));
}

TEST_F(rask_cst_Parser, variableDeclarations)
{
    using namespace rask;

    source << "main() -> void\n{\n    var x;\n    var y = -2;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(2u, f.stmts.size());

    cst::VariableDecl& decl1 = getVariableDecl(f.stmts[0]);
    ASSERT_IDENTIFIER("x", at(3, 9), decl1.name);
    ASSERT_TRUE(!decl1.value);

    cst::VariableDecl& decl2 = getVariableDecl(f.stmts[1]);
    ASSERT_IDENTIFIER("y", at(4, 9), decl2.name);
    ASSERT_TRUE(decl2.value);
    ASSERT_CONST(-2, at(4, 13), *decl2.value);
}

TEST_F(rask_cst_Parser, functionCallsWithVariables)
{
    using namespace rask;

    source << "main() -> void\n{\n    f(x, 1, y);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());

    cst::FunctionCall& call = getFunctionCall(f.stmts[0]);
    ASSERT_IDENTIFIER("f", at(3, 5), call.function);
    ASSERT_EQ(3u, call.args.size());
    ASSERT_VARIABLE("x", at(3, 7), call.args[0]);
    ASSERT_CONST(1, at(3, 10), call.args[1]);
    ASSERT_VARIABLE("y", at(3, 13), call.args[2]);
}

TEST_F(rask_cst_Parser, voidFunctionDeclarations)
{
    using namespace rask;

    source << "f1() -> void\n{\n}\nf2() -> void\n{\n}\n";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(2u, tree->functions.size());

    cst::Function& f1 = tree->functions[0];
    ASSERT_IDENTIFIER("f1", at(1, 1), f1.name);
    ASSERT_EQ(0u, f1.stmts.size());

    cst::Function& f2 = tree->functions[1];
    ASSERT_IDENTIFIER("f2", at(4, 1), f2.name);
    ASSERT_EQ(0u, f2.stmts.size());
}

TEST_F(rask_cst_Parser, variableInitializedByVariable)
{
    using namespace rask;

    source << "main() -> void\n{\n    var a = b;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());

    cst::VariableDecl& decl = getVariableDecl(f.stmts[0]);
    ASSERT_IDENTIFIER("a", at(3, 9), decl.name);
    ASSERT_TRUE(decl.value);
    ASSERT_VARIABLE("b", at(3, 13), *decl.value);
}

TEST_F(rask_cst_Parser, functionDeclarationWithArguments)
{
    using namespace rask;

    source << "f(Type1 x, Type2 y, Type3 z) -> void\n{ }";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_IDENTIFIER("f", at(1, 1), f.name);
    ASSERT_EQ(3u, f.args.size());
    ASSERT_IDENTIFIER("x", at(1, 9), f.args[0].name);
    ASSERT_IDENTIFIER("Type1", at(1, 3), f.args[0].type);
    ASSERT_IDENTIFIER("y", at(1, 18), f.args[1].name);
    ASSERT_IDENTIFIER("Type2", at(1, 12), f.args[1].type);
    ASSERT_IDENTIFIER("z", at(1, 27), f.args[2].name);
    ASSERT_IDENTIFIER("Type3", at(1, 21), f.args[2].type);
    ASSERT_TRUE(f.stmts.empty());
}

TEST_F(rask_cst_Parser, functionDeclarationWithReturnType)
{
    using namespace rask;

    source << "f() -> abcd123\n{ }";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());
    ASSERT_IDENTIFIER("abcd123", at(1, 8), tree->functions[0].type);
}

TEST_F(rask_cst_Parser, returnStatements)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return 10;\n    return x;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_IDENTIFIER("abcd123", at(1, 8), tree->functions[0].type);
    ASSERT_EQ(2u, f.stmts.size());
    cst::Return& ret1 = getReturn(f.stmts[0]);
    ASSERT_EQ(at(3, 5), ret1.position);
    ASSERT_CONST(10, at(3, 12), ret1.value);
    cst::Return& ret2 = getReturn(f.stmts[1]);
    ASSERT_EQ(at(4, 5), ret2.position);
    ASSERT_VARIABLE("x", at(4, 12), ret2.value);
}

TEST_F(rask_cst_Parser, variableInitializedByFunctionCall)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    var a = g(10, x);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::FunctionCall& fc = getFunctionCall(*getVariableDecl(f.stmts[0]).value);
    ASSERT_IDENTIFIER("g", at(3, 13), fc.function);
    ASSERT_EQ(2u, fc.args.size());
    ASSERT_CONST(10, at(3, 15), fc.args[0]);
    ASSERT_VARIABLE("x", at(3, 19), fc.args[1]);
}

TEST_F(rask_cst_Parser, functionCallWithFunctionCallAsArgument)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    h(g(5));\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    cst::FunctionCall& fc1 = getFunctionCall(f.stmts[0]);
    ASSERT_EQ(1u, fc1.args.size());
    cst::FunctionCall& fc2 = getFunctionCall(fc1.args[0]);
    ASSERT_IDENTIFIER("g", at(3, 7), fc2.function);
    ASSERT_EQ(1u, fc2.args.size());
    ASSERT_CONST(5, at(3, 9), fc2.args[0]);
}

TEST_F(rask_cst_Parser, returnWithFunctionCall)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return g();\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::Return& ret = getReturn(f.stmts[0]);
    ASSERT_EQ(at(3, 5), ret.position);
    const cst::FunctionCall& fc = getFunctionCall(ret.value);
    ASSERT_IDENTIFIER("g", at(3, 12), fc.function);
    ASSERT_EQ(0u, fc.args.size());
}

TEST_F(rask_cst_Parser, unaryMinusOnVariable)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return -x;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::UnaryOperatorCall& e = getUnaryOperatorCall(getReturn(f.stmts[0]).value);
    ASSERT_OPERATOR(cst::UnaryOperator::MINUS, at(3, 12), e);
    ASSERT_VARIABLE("x", at(3, 13), e.expr);
}

TEST_F(rask_cst_Parser, returnAdditionAndSubtraction)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getReturn(f.stmts[0]).value);
    ASSERT_VARIABLE("x", at(3, 12), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 14), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 16), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 18), expr.next[1]);
    ASSERT_CONST(5, at(3, 20), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, variableInitializationByAdditionAndSubtraction)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    var z = x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(*getVariableDecl(f.stmts[0]).value);
    ASSERT_VARIABLE("x", at(3, 13), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 15), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 17), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 19), expr.next[1]);
    ASSERT_CONST(5, at(3, 21), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithAdditionAndSubtraction)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x + y - 5);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 13), expr.next[1]);
    ASSERT_CONST(5, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithMultiplication)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x * y * 7);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithDivision)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x / y / 7);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithModulo)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x % y % 7);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MOD, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MOD, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, operatorPriorities)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x + y * 7 / 9 % 3 - z);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 9), expr.next[0]);

    const cst::ChainExpression expr2 = getChainExpression(expr.next[0].expr);
    ASSERT_VARIABLE("y", at(3, 11), expr2.expr);
    ASSERT_EQ(3u, expr2.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 13), expr2.next[0]);
    ASSERT_CONST(7, at(3, 15), expr2.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 17), expr2.next[1]);
    ASSERT_CONST(9, at(3, 19), expr2.next[1].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MOD, at(3, 21), expr2.next[2]);
    ASSERT_CONST(3, at(3, 23), expr2.next[2].expr);

    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 25), expr.next[1]);
    ASSERT_VARIABLE("z", at(3, 27), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, parensInArithmeicExpression)
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f((x + y) * (u - v) / (a * b));\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ASSERT_TRUE(tree);
    ASSERT_NO_ERRORS();
    ASSERT_EQ(1u, tree->functions.size());

    cst::Function& f = tree->functions[0];
    ASSERT_EQ(1u, f.stmts.size());
    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(f.stmts[0]).args[0]);
    const cst::ChainExpression& left = getChainExpression(expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 15), expr.next[0]);
    const cst::ChainExpression& center = getChainExpression(expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 25), expr.next[1]);
    const cst::ChainExpression& right = getChainExpression(expr.next[1].expr);

    ASSERT_VARIABLE("x", at(3, 8), left.expr);
    ASSERT_EQ(1u, left.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 10), left.next[0]);
    ASSERT_VARIABLE("y", at(3, 12), left.next[0].expr);

    ASSERT_VARIABLE("u", at(3, 18), center.expr);
    ASSERT_EQ(1u, center.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 20), center.next[0]);
    ASSERT_VARIABLE("v", at(3, 22), center.next[0].expr);

    ASSERT_VARIABLE("a", at(3, 28), right.expr);
    ASSERT_EQ(1u, right.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 30), right.next[0]);
    ASSERT_VARIABLE("b", at(3, 32), right.next[0].expr);
}

