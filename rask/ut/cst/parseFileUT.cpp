// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <algorithm>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/test/TUTAssert.hpp>
#include <rask/cst/parseFile.hpp>

#define ENSURE_IDENTIFIER(actual, expectedValue, expectedPosition) \
    ENSURE_EQUALS((actual).value, expectedValue); \
    ENSURE_EQUALS((actual).position, expectedPosition)
    
#define ENSURE_CONST(actual, expectedValue, expectedPosition) \
    ENSURE_EQUALS(getConstant((actual).expr).value, expectedValue); \
    ENSURE_EQUALS(getConstant((actual).expr).position, expectedPosition)

#define ENSURE_VARIABLE(actual, expectedName, expectedPosition) \
    ENSURE_IDENTIFIER(getIdentifier((actual).expr), expectedName, expectedPosition)

#define ENSURE_OPERATOR(actual, expectedTag, expectedPosition) \
    ENSURE((actual).op.tag == expectedTag); \
    ENSURE_EQUALS((actual).op.position, expectedPosition)

#define ENSURE_NO_ERRORS() \
    ENSURE_EQUALS(errorLogger.errors().size(), 0u)

namespace tut
{

struct parseMain_TestData
{
    std::stringstream source;
    rask::error::Logger errorLogger;
    rask::InputStream sourceStream;

    parseMain_TestData() : sourceStream("test.rask", source) { }

    void ensureErrorCountEquals(unsigned n)
    {
        ensure_equals("error count", errorLogger.errors().size(), n);
    }

    void ensureError(const rask::error::Message& msg)
    {
        std::ostringstream info;

        info << "expected \'" << msg << "\'";

        ensure(info.str(), std::find(errorLogger.errors().begin(), errorLogger.errors().end(), msg) != errorLogger.errors().end());
    }

    rask::Position at(unsigned row, unsigned line)
    {
        return rask::Position(sourceStream.file(), row, line);
    }

    boost::optional<rask::cst::Tree> parseFile()
    {
        return rask::cst::parseFile(sourceStream, errorLogger);
    }
};

typedef test_group<parseMain_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cst.parseFile");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    source << "main() -> void\n{\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_EQUALS(tree->functions.size(), 1u);
    ENSURE_IDENTIFIER(tree->functions[0].name, "main", at(1, 1));
    ENSURE_NO_ERRORS();
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingMainFunction(Position(sourceStream.file())));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    source << "main() -> \n{\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingReturnType(at(2, 1)));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    source << "main() -> void\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningBrace(at(2, 1)));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    source << "main() -> void\n{\n";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingBrace(at(3, 1)));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    source << "main) -> void\n{\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(at(1, 5)));
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;

    source << "main( -> void\n{\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingParen(at(1, 7)));
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;

    source << "main() - void\n{\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(at(1, 8)));
}

template <>
template <>
void object::test<9>()
{
    using namespace rask;

    source << "main() > void\n{\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(at(1, 8)));
}

template <>
template <>
void object::test<10>()
{
    using namespace rask;

    source << "main() -> void\n{\n    abcd();\n    efgh(-2);\n    ijkl(2, 3);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    cst::Function& f = tree->functions[0]; 
    ENSURE_IDENTIFIER(f.name, "main", at(1, 1));
    ENSURE_EQUALS(f.stmts.size(), 3u);

    cst::FunctionCall& fc1 = getFunctionCall(f.stmts[0]);
    ENSURE_IDENTIFIER(fc1.function, "abcd", at(3, 5));
    ENSURE_EQUALS(fc1.args.size(), 0u);

    cst::FunctionCall& fc2 = getFunctionCall(f.stmts[1]);
    ENSURE_IDENTIFIER(fc2.function, "efgh", at(4, 5));
    ENSURE_EQUALS(fc2.args.size(), 1u);
    ENSURE_CONST(fc2.args[0], -2, at(4, 10));

    cst::FunctionCall& fc3 = getFunctionCall(f.stmts[2]);
    ENSURE_IDENTIFIER(fc3.function, "ijkl", at(5, 5));
    ENSURE_EQUALS(fc3.args.size(), 2u);
    ENSURE_CONST(fc3.args[0], 2, at(5, 10));
    ENSURE_CONST(fc3.args[1], 3, at(5, 13));
}

template <>
template <>
void object::test<11>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print(1)\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingSemicolon(at(4, 1)));
}

template <>
template <>
void object::test<12>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print;\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(at(3, 10)));
}

template <>
template <>
void object::test<13>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print(5 5;\n}";

    ENSURE(!parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingParen(at(3, 13)));
}

template <>
template <>
void object::test<14>()
{
    using namespace rask;

    source << "main() -> void\n{\n    var x;\n    var y = -2;\n}";

    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 2u);

    cst::VariableDecl& decl1 = getVariableDecl(f.stmts[0]);
    ENSURE_IDENTIFIER(decl1.name, "x", at(3, 9));
    ENSURE(!decl1.value);

    cst::VariableDecl& decl2 = getVariableDecl(f.stmts[1]);
    ENSURE_IDENTIFIER(decl2.name, "y", at(4, 9));
    ENSURE(decl2.value);
    ENSURE_CONST(*decl2.value, -2, at(4, 13));
}

template <>
template <>
void object::test<15>()
{
    using namespace rask;

    source << "main() -> void\n{\n    f(x, 1, y);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);

    cst::FunctionCall& call = getFunctionCall(f.stmts[0]);
    ENSURE_IDENTIFIER(call.function, "f", at(3, 5));
    ENSURE_EQUALS(call.args.size(), 3u);
    ENSURE_VARIABLE(call.args[0], "x", at(3, 7));
    ENSURE_CONST(call.args[1], 1, at(3, 10));
    ENSURE_VARIABLE(call.args[2], "y", at(3, 13));
}

template <>
template <>
void object::test<16>()
{
    using namespace rask;
    
    source << "f1() -> void\n{\n}\nf2() -> void\n{\n}\n";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 2u);

    cst::Function& f1 = tree->functions[0];
    ENSURE_IDENTIFIER(f1.name, "f1", at(1, 1));
    ENSURE_EQUALS(f1.stmts.size(), 0u);
    
    cst::Function& f2 = tree->functions[1];
    ENSURE_IDENTIFIER(f2.name, "f2", at(4, 1));
    ENSURE_EQUALS(f2.stmts.size(), 0u);
}

template <>
template <>
void object::test<17>()
{
    using namespace rask;
    
    source << "main() -> void\n{\n    var a = b;\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);

    cst::VariableDecl& decl = getVariableDecl(f.stmts[0]);
    ENSURE_IDENTIFIER(decl.name, "a", at(3, 9));
    ENSURE(decl.value);
    ENSURE_VARIABLE(*decl.value, "b", at(3, 13));
}

template <>
template <>
void object::test<18>()
{
    using namespace rask;
    
    source << "f(int32 x, int32 y, int32 z) -> void\n{ }";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_IDENTIFIER(f.name, "f", at(1, 1));
    ENSURE_EQUALS(f.args.size(), 3u);
    ENSURE_IDENTIFIER(f.args[0], "x", at(1, 9));
    ENSURE_IDENTIFIER(f.args[1], "y", at(1, 18));
    ENSURE_IDENTIFIER(f.args[2], "z", at(1, 27));
    ENSURE(f.stmts.empty());
}

template <>
template <>
void object::test<19>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{ }";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    ENSURE_IDENTIFIER(tree->functions[0].type, "abcd123", at(1, 8));
}

template <>
template <>
void object::test<20>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    return 10;\n    return x;\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_IDENTIFIER(tree->functions[0].type, "abcd123", at(1, 8));
    ENSURE_EQUALS(f.stmts.size(), 2u);
    cst::Return& ret1 = getReturn(f.stmts[0]);
    ENSURE_EQUALS(ret1.position, at(3, 5));
    ENSURE_CONST(ret1.value, 10, at(3, 12));
    cst::Return& ret2 = getReturn(f.stmts[1]);
    ENSURE_EQUALS(ret2.position, at(4, 5));
    ENSURE_VARIABLE(ret2.value, "x", at(4, 12));
}

template <>
template <>
void object::test<21>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    var a = g(10, x);\n}";

    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::FunctionCall& fc = getFunctionCall(getVariableDecl(f.stmts[0]).value->expr);
    ENSURE_IDENTIFIER(fc.function, "g", at(3, 13));
    ENSURE_EQUALS(fc.args.size(), 2u);
    ENSURE_CONST(fc.args[0], 10, at(3, 15));
    ENSURE_VARIABLE(fc.args[1], "x", at(3, 19));
}

template <>
template <>
void object::test<22>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    h(g(5));\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    cst::FunctionCall& fc1 = getFunctionCall(f.stmts[0]);
    ENSURE_EQUALS(fc1.args.size(), 1u);
    cst::FunctionCall& fc2 = getFunctionCall(fc1.args[0].expr);
    ENSURE_IDENTIFIER(fc2.function, "g", at(3, 7));
    ENSURE_EQUALS(fc2.args.size(), 1u);
    ENSURE_CONST(fc2.args[0], 5, at(3, 9));
}

template <>
template <>
void object::test<23>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    return g();\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Return& ret = getReturn(f.stmts[0]);
    ENSURE_EQUALS(ret.position, at(3, 5));
    const cst::FunctionCall& fc = getFunctionCall(ret.value.expr);
    ENSURE(ret.value.next.empty());
    ENSURE_IDENTIFIER(fc.function, "g", at(3, 12));
    ENSURE_EQUALS(fc.args.size(), 0u);
}

template <>
template <>
void object::test<24>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    return -x;\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::UnaryOperatorCall& e = getUnaryOperatorCall(getReturn(f.stmts[0]).value.expr);
    ENSURE_OPERATOR(e, cst::UnaryOperator::MINUS, at(3, 12));
    ENSURE_VARIABLE(e, "x", at(3, 13));
}

template <>
template <>
void object::test<25>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Expression& expr = getReturn(f.stmts[0]).value;
    ENSURE_VARIABLE(expr, "x", at(3, 12));
    ENSURE_EQUALS(expr.next.size(), 2u);
    ENSURE_OPERATOR(expr.next[0], cst::BinaryOperator::PLUS, at(3, 14));
    ENSURE_VARIABLE(expr.next[0], "y", at(3, 16));
    ENSURE_OPERATOR(expr.next[1], cst::BinaryOperator::MINUS, at(3, 18));
    ENSURE_CONST(expr.next[1], 5, at(3, 20));
}

template <>
template <>
void object::test<26>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    var z = x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Expression& expr = *getVariableDecl(f.stmts[0]).value;
    ENSURE_VARIABLE(expr, "x", at(3, 13));
    ENSURE_EQUALS(expr.next.size(), 2u);
    ENSURE_OPERATOR(expr.next[0], cst::BinaryOperator::PLUS, at(3, 15));
    ENSURE_VARIABLE(expr.next[0], "y", at(3, 17));
    ENSURE_OPERATOR(expr.next[1], cst::BinaryOperator::MINUS, at(3, 19));
    ENSURE_CONST(expr.next[1], 5, at(3, 21));
}

template <>
template <>
void object::test<27>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x + y - 5);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Expression& expr = getFunctionCall(f.stmts[0]).args[0];
    ENSURE_VARIABLE(expr, "x", at(3, 7));
    ENSURE_EQUALS(expr.next.size(), 2u);
    ENSURE_OPERATOR(expr.next[0], cst::BinaryOperator::PLUS, at(3, 9));
    ENSURE_VARIABLE(expr.next[0], "y", at(3, 11));
    ENSURE_OPERATOR(expr.next[1], cst::BinaryOperator::MINUS, at(3, 13));
    ENSURE_CONST(expr.next[1], 5, at(3, 15));
}

template <>
template <>
void object::test<28>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    f(x * y * 7);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);

    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Expression& expr = getFunctionCall(f.stmts[0]).args[0];
    ENSURE_VARIABLE(expr, "x", at(3, 7));
    ENSURE_EQUALS(expr.next.size(), 2u);
    ENSURE_OPERATOR(expr.next[0], cst::BinaryOperator::MULT, at(3, 9));
    ENSURE_VARIABLE(expr.next[0], "y", at(3, 11));
    ENSURE_OPERATOR(expr.next[1], cst::BinaryOperator::MULT, at(3, 13));
    ENSURE_CONST(expr.next[1], 7, at(3, 15));
}

template <>
template <>
void object::test<29>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    f(x / y / 7);\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ENSURE_NO_ERRORS();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    cst::Function& f = tree->functions[0];
    ENSURE_EQUALS(f.stmts.size(), 1u);
    const cst::Expression& expr = getFunctionCall(f.stmts[0]).args[0];
    ENSURE_VARIABLE(expr, "x", at(3, 7));
    ENSURE_EQUALS(expr.next.size(), 2u);
    ENSURE_OPERATOR(expr.next[0], cst::BinaryOperator::DIV, at(3, 9));
    ENSURE_VARIABLE(expr.next[0], "y", at(3, 11));
    ENSURE_OPERATOR(expr.next[1], cst::BinaryOperator::DIV, at(3, 13));
    ENSURE_CONST(expr.next[1], 7, at(3, 15));
}

}
