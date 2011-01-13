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

using namespace rask;

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
    error::Logger errorLogger;
    std::string file;
    cst::Parser parser;
    cst::Tree tree;
    cst::Function function;

    rask_cst_Parser()
        : file("test.rask"), parser(errorLogger) { }

    void assertParseError(const error::Message& msg)
    {
        ASSERT_FALSE(parseFile());
        ASSERT_EQ(1u, errorLogger.errors().size());
        ASSERT_EQ(msg, errorLogger.errors()[0]);
    }

    void parseOneFunction()
    {
        boost::optional<cst::Tree> tree = parseFile();
        ASSERT_TRUE(tree);
        ASSERT_NO_ERRORS();
        ASSERT_EQ(1u, tree->functions.size());
        function = tree->functions[0];
        this->tree = *tree;
    }

    void parseOneFunctionWithStmts(unsigned numStmts)
    {
        parseOneFunction();
        ASSERT_EQ(numStmts, function.stmts.size());
    }

    Position at(unsigned row, unsigned line)
    {
        return Position(file, row, line);
    }

    boost::optional<cst::Tree> parseFile()
    {
        InputStream sourceStream(file, source);
        return parser.parseFile(sourceStream);
    }
};

TEST_F(rask_cst_Parser, onlyMain)
{
    source << "main() -> void\n{\n}";

    parseOneFunction();
    ASSERT_IDENTIFIER("main", at(1, 1), function.name);
}

TEST_F(rask_cst_Parser, emptySource)
{
    assertParseError(error::Message::missingMainFunction(Position(file)));
}

TEST_F(rask_cst_Parser, noReturnTypeAfterMain)
{
    source << "main() -> \n{\n}";

    assertParseError(error::Message::missingReturnType(at(2, 1)));
}

TEST_F(rask_cst_Parser, noOpeningBraceAfterMain)
{
    source << "main() -> void\n}";

    assertParseError(error::Message::missingOpeningBrace(at(2, 1)));
}

TEST_F(rask_cst_Parser, noClosingBraceAfterMain)
{
    source << "main() -> void\n{\n";

    assertParseError(error::Message::missingClosingBrace(at(3, 1)));
}

TEST_F(rask_cst_Parser, noOpeningParenInMain)
{
    source << "main) -> void\n{\n}";

    assertParseError(error::Message::missingOpeningParen(at(1, 5)));
}

TEST_F(rask_cst_Parser, noClosingParenInMain)
{
    source << "main( -> void\n{\n}";

    assertParseError(error::Message::missingClosingParen(at(1, 7)));
}

TEST_F(rask_cst_Parser, noGtInRightArrowInMain)
{
    source << "main() - void\n{\n}";

    assertParseError(error::Message::missingRightArrow(at(1, 8)));
}

TEST_F(rask_cst_Parser, noHyphenInRightArrowInMain)
{
    source << "main() > void\n{\n}";

    assertParseError(error::Message::missingRightArrow(at(1, 8)));
}

TEST_F(rask_cst_Parser, functionCallsInMain)
{
    source << "main() -> void\n{\n    abcd();\n    efgh(-2);\n    ijkl(2, 3);\n}";

    parseOneFunctionWithStmts(3);
    ASSERT_IDENTIFIER("main", at(1, 1), function.name);

    cst::FunctionCall& fc1 = getFunctionCall(function.stmts[0]);
    ASSERT_IDENTIFIER("abcd", at(3, 5), fc1.function);
    ASSERT_EQ(0u, fc1.args.size());

    cst::FunctionCall& fc2 = getFunctionCall(function.stmts[1]);
    ASSERT_IDENTIFIER("efgh", at(4, 5), fc2.function);
    ASSERT_EQ(1u, fc2.args.size());
    ASSERT_CONST(-2, at(4, 10), fc2.args[0]);

    cst::FunctionCall& fc3 = getFunctionCall(function.stmts[2]);
    ASSERT_IDENTIFIER("ijkl", at(5, 5), fc3.function);
    ASSERT_EQ(2u, fc3.args.size());
    ASSERT_CONST(2, at(5, 10), fc3.args[0]);
    ASSERT_CONST(3, at(5, 13), fc3.args[1]);
}

TEST_F(rask_cst_Parser, noSemicolonAfternFunctionCall)
{
    source << "main() -> void\n{\n    print(1)\n}";

    assertParseError(error::Message::missingSemicolon(at(4, 1)));
}

TEST_F(rask_cst_Parser, noOpeningParenAfterFunctionCall)
{
    source << "main() -> void\n{\n    print;\n}";

    assertParseError(error::Message::missingOpeningParen(at(3, 10)));
}

TEST_F(rask_cst_Parser, noClosingParenAfterFunctionCall)
{
    source << "main() -> void\n{\n    print(5 5;\n}";

    assertParseError(error::Message::missingClosingParen(at(3, 13)));
}

TEST_F(rask_cst_Parser, variableDeclarations)
{
    source << "main() -> void\n{\n    var x;\n    var y = -2;\n}";

    parseOneFunctionWithStmts(2);

    cst::VariableDecl& decl1 = getVariableDecl(function.stmts[0]);
    ASSERT_IDENTIFIER("x", at(3, 9), decl1.name);
    ASSERT_TRUE(!decl1.value);

    cst::VariableDecl& decl2 = getVariableDecl(function.stmts[1]);
    ASSERT_IDENTIFIER("y", at(4, 9), decl2.name);
    ASSERT_TRUE(decl2.value);
    ASSERT_CONST(-2, at(4, 13), *decl2.value);
}

TEST_F(rask_cst_Parser, functionCallsWithVariables)
{
    source << "main() -> void\n{\n    f(x, 1, y);\n}";

    parseOneFunctionWithStmts(1);

    cst::FunctionCall& call = getFunctionCall(function.stmts[0]);
    ASSERT_IDENTIFIER("f", at(3, 5), call.function);
    ASSERT_EQ(3u, call.args.size());
    ASSERT_VARIABLE("x", at(3, 7), call.args[0]);
    ASSERT_CONST(1, at(3, 10), call.args[1]);
    ASSERT_VARIABLE("y", at(3, 13), call.args[2]);
}

TEST_F(rask_cst_Parser, voidFunctionDeclarations)
{
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
    source << "main() -> void\n{\n    var a = b;\n}";

    parseOneFunctionWithStmts(1);

    cst::VariableDecl& decl = getVariableDecl(function.stmts[0]);
    ASSERT_IDENTIFIER("a", at(3, 9), decl.name);
    ASSERT_TRUE(decl.value);
    ASSERT_VARIABLE("b", at(3, 13), *decl.value);
}

TEST_F(rask_cst_Parser, functionDeclarationWithArguments)
{
    source << "f(Type1 x, Type2 y, Type3 z) -> void\n{ }";

    parseOneFunction();

    ASSERT_IDENTIFIER("f", at(1, 1), function.name);
    ASSERT_EQ(3u, function.args.size());
    ASSERT_IDENTIFIER("x", at(1, 9), function.args[0].name);
    ASSERT_IDENTIFIER("Type1", at(1, 3), function.args[0].type);
    ASSERT_IDENTIFIER("y", at(1, 18), function.args[1].name);
    ASSERT_IDENTIFIER("Type2", at(1, 12), function.args[1].type);
    ASSERT_IDENTIFIER("z", at(1, 27), function.args[2].name);
    ASSERT_IDENTIFIER("Type3", at(1, 21), function.args[2].type);
    ASSERT_TRUE(function.stmts.empty());
}

TEST_F(rask_cst_Parser, functionDeclarationWithReturnType)
{
    source << "f() -> abcd123\n{ }";

    parseOneFunction();
    ASSERT_IDENTIFIER("abcd123", at(1, 8), function.type);
}

TEST_F(rask_cst_Parser, returnStatements)
{
    source << "f() -> abcd123\n{\n    return 10;\n    return x;\n}";

    parseOneFunctionWithStmts(2);

    cst::Return& ret1 = getReturn(function.stmts[0]);
    ASSERT_EQ(at(3, 5), ret1.position);
    ASSERT_CONST(10, at(3, 12), ret1.value);
    cst::Return& ret2 = getReturn(function.stmts[1]);
    ASSERT_EQ(at(4, 5), ret2.position);
    ASSERT_VARIABLE("x", at(4, 12), ret2.value);
}

TEST_F(rask_cst_Parser, variableInitializedByFunctionCall)
{
    source << "f() -> abcd123\n{\n    var a = g(10, x);\n}";

    parseOneFunctionWithStmts(1);

    const cst::FunctionCall& fc = getFunctionCall(*getVariableDecl(function.stmts[0]).value);
    ASSERT_IDENTIFIER("g", at(3, 13), fc.function);
    ASSERT_EQ(2u, fc.args.size());
    ASSERT_CONST(10, at(3, 15), fc.args[0]);
    ASSERT_VARIABLE("x", at(3, 19), fc.args[1]);
}

TEST_F(rask_cst_Parser, functionCallWithFunctionCallAsArgument)
{
    source << "f() -> abcd123\n{\n    h(g(5));\n}";

    parseOneFunctionWithStmts(1);

    cst::FunctionCall& fc1 = getFunctionCall(function.stmts[0]);
    ASSERT_EQ(1u, fc1.args.size());
    cst::FunctionCall& fc2 = getFunctionCall(fc1.args[0]);
    ASSERT_IDENTIFIER("g", at(3, 7), fc2.function);
    ASSERT_EQ(1u, fc2.args.size());
    ASSERT_CONST(5, at(3, 9), fc2.args[0]);
}

TEST_F(rask_cst_Parser, returnWithFunctionCall)
{
    source << "f() -> abcd123\n{\n    return g();\n}";

    parseOneFunctionWithStmts(1);

    const cst::Return& ret = getReturn(function.stmts[0]);
    ASSERT_EQ(at(3, 5), ret.position);
    const cst::FunctionCall& fc = getFunctionCall(ret.value);
    ASSERT_IDENTIFIER("g", at(3, 12), fc.function);
    ASSERT_EQ(0u, fc.args.size());
}

TEST_F(rask_cst_Parser, unaryMinusOnVariable)
{
    source << "f() -> abcd123\n{\n    return -x;\n}";

    parseOneFunctionWithStmts(1);

    const cst::UnaryOperatorCall& e = getUnaryOperatorCall(getReturn(function.stmts[0]).value);
    ASSERT_OPERATOR(cst::UnaryOperator::MINUS, at(3, 12), e);
    ASSERT_VARIABLE("x", at(3, 13), e.expr);
}

TEST_F(rask_cst_Parser, returnAdditionAndSubtraction)
{
    source << "f() -> abcd123\n{\n    return x + y - 5;\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getReturn(function.stmts[0]).value);
    ASSERT_VARIABLE("x", at(3, 12), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 14), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 16), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 18), expr.next[1]);
    ASSERT_CONST(5, at(3, 20), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, variableInitializationByAdditionAndSubtraction)
{
    source << "f() -> abcd123\n{\n    var z = x + y - 5;\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(*getVariableDecl(function.stmts[0]).value);
    ASSERT_VARIABLE("x", at(3, 13), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 15), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 17), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 19), expr.next[1]);
    ASSERT_CONST(5, at(3, 21), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithAdditionAndSubtraction)
{
    source << "f() -> abcd123\n{\n    f(x + y - 5);\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::PLUS, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MINUS, at(3, 13), expr.next[1]);
    ASSERT_CONST(5, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithMultiplication)
{
    source << "f() -> abcd123\n{\n    f(x * y * 7);\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MULT, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithDivision)
{
    source << "f() -> abcd123\n{\n    f(x / y / 7);\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::DIV, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, functionCallWithModulo)
{
    source << "f() -> abcd123\n{\n    f(x % y % 7);\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
    ASSERT_VARIABLE("x", at(3, 7), expr.expr);
    ASSERT_EQ(2u, expr.next.size());
    ASSERT_OPERATOR(cst::BinaryOperator::MOD, at(3, 9), expr.next[0]);
    ASSERT_VARIABLE("y", at(3, 11), expr.next[0].expr);
    ASSERT_OPERATOR(cst::BinaryOperator::MOD, at(3, 13), expr.next[1]);
    ASSERT_CONST(7, at(3, 15), expr.next[1].expr);
}

TEST_F(rask_cst_Parser, operatorPriorities)
{
    source << "f() -> abcd123\n{\n    f(x + y * 7 / 9 % 3 - z);\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
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
    source << "f() -> abcd123\n{\n    f((x + y) * (u - v) / (a * b));\n}";

    parseOneFunctionWithStmts(1);

    const cst::ChainExpression& expr = getChainExpression(getFunctionCall(function.stmts[0]).args[0]);
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

