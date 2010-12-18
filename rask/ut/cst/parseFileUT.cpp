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

namespace tut
{

struct parseMain_TestData
{
    std::stringstream source;
    rask::error::Logger errorLogger;
    rask::InputStream sourceStream;

    parseMain_TestData() : sourceStream("test.rask", source) { }

    void ensureNoErrors()
    {
        ensure("no errors", errorLogger.errors().empty());
    }

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

    ensure("parsed", tree);
    ensure_size("functions", tree->functions, 1u);
    ensure_equals("name", tree->functions[0].name.value, "main");
    ensure_equals("name pos", tree->functions[0].name.position, at(1, 1));
    ensureNoErrors();
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingMainFunction(Position(sourceStream.file())));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    source << "main() -> \n{\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingReturnType(at(2, 1)));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    source << "main() -> void\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningBrace(at(2, 1)));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    source << "main() -> void\n{\n";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingBrace(at(3, 1)));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    source << "main) -> void\n{\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(at(1, 5)));
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;

    source << "main( -> void\n{\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingParen(at(1, 7)));
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;

    source << "main() - void\n{\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(at(1, 8)));
}

template <>
template <>
void object::test<9>()
{
    using namespace rask;

    source << "main() > void\n{\n}";

    ensure_not("not parsed", parseFile());
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

    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    ensure_equals("name", tree->functions[0].name.value, "main");
    ensure_equals("name pos", tree->functions[0].name.position, at(1, 1));
    ensure_equals("count", tree->functions[0].stmts.size(), 3u);

    ensure_equals("function 1", getFunctionCall(tree->functions[0].stmts[0]).function.value, "abcd");
    ensure_equals("function pos 1", getFunctionCall(tree->functions[0].stmts[0]).function.position, at(3, 5));
    ensure_equals("count 1", getFunctionCall(tree->functions[0].stmts[0]).args.size(), 0u);

    ensure_equals("function 2", getFunctionCall(tree->functions[0].stmts[1]).function.value, "efgh");
    ensure_equals("function pos 2", getFunctionCall(tree->functions[0].stmts[1]).function.position, at(4, 5));
    ensure_equals("count 2", getFunctionCall(tree->functions[0].stmts[1]).args.size(), 1u);
    ensure_equals("value 2", getConstant(getFunctionCall(tree->functions[0].stmts[1]).args[0]).value, -2);
    ensure_equals("value 2 pos", getConstant(getFunctionCall(tree->functions[0].stmts[1]).args[0]).position, at(4, 10));

    ensure_equals("function 3", getFunctionCall(tree->functions[0].stmts[2]).function.value, "ijkl");
    ensure_equals("function pos 3", getFunctionCall(tree->functions[0].stmts[2]).function.position, at(5, 5));
    ensure_equals("count 3", getFunctionCall(tree->functions[0].stmts[2]).args.size(), 2u);
    ensure_equals("value 3", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[0]).value, 2);
    ensure_equals("value 3 pos", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[0]).position, at(5, 10));
    ensure_equals("value 4", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[1]).value, 3);
    ensure_equals("value 4 pos", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[1]).position, at(5, 13));
}

template <>
template <>
void object::test<11>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print(1)\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingSemicolon(at(4, 1)));
}

template <>
template <>
void object::test<12>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print;\n}";

    ensure_not("not parsed", parseFile());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(at(3, 10)));
}

template <>
template <>
void object::test<13>()
{
    using namespace rask;

    source << "main() -> void\n{\n    print(5 5;\n}";

    ensure_not("not parsed", parseFile());
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
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_equals("statements", tree->functions[0].stmts.size(), 2u);

    ensure_equals("x position", getVariableDecl(tree->functions[0].stmts[0]).name.position, at(3, 9));
    ensure_equals("x name", getVariableDecl(tree->functions[0].stmts[0]).name.value, "x");
    ensure_not("no x value", getVariableDecl(tree->functions[0].stmts[0]).value);

    ensure_equals("y position", getVariableDecl(tree->functions[0].stmts[1]).name.position, at(4, 9));
    ensure_equals("y name", getVariableDecl(tree->functions[0].stmts[1]).name.value, "y");
    ensure("y has value", getVariableDecl(tree->functions[0].stmts[1]).value);
    ensure_equals("y value", getConstant(getVariableDecl(tree->functions[0].stmts[1]).value->first).value, -2);
    ensure_equals("y value position", getConstant(getVariableDecl(tree->functions[0].stmts[1]).value->first).position, at(4, 13));
}

template <>
template <>
void object::test<15>()
{
    using namespace rask;

    source << "main() -> void\n{\n    f(x, 1, y);\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_size("statements", tree->functions[0].stmts, 1u);
    ensure_equals("name", getFunctionCall(tree->functions[0].stmts[0]).function.value, "f");
    ensure_equals("func pos", getFunctionCall(tree->functions[0].stmts[0]).function.position, at(3, 5));
    ensure_size("args", getFunctionCall(tree->functions[0].stmts[0]).args, 3u);
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[0]).value, "x");
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[0]).position, at(3, 7));
    ensure_equals(getConstant(getFunctionCall(tree->functions[0].stmts[0]).args[1]).value, 1);
    ensure_equals(getConstant(getFunctionCall(tree->functions[0].stmts[0]).args[1]).position, at(3, 10));
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[2]).value, "y");
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[2]).position, at(3, 13));
}

template <>
template <>
void object::test<16>()
{
    using namespace rask;
    
    source << "f1() -> void\n{\n}\nf2() -> void\n{\n}\n";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 2u);

    ensure_equals(tree->functions[0].name.value, "f1");
    ensure_equals(tree->functions[0].name.position, at(1, 1));
    ensure_empty("f1 no stmts", tree->functions[0].stmts);

    ensure_equals(tree->functions[1].name.value, "f2");
    ensure_equals(tree->functions[1].name.position, at(4, 1));
    ensure_empty("f2 no stmts", tree->functions[1].stmts);
}

template <>
template <>
void object::test<17>()
{
    using namespace rask;
    
    source << "main() -> void\n{\n    var a = b;\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_equals("statements", tree->functions[0].stmts.size(), 1u);
   
    ensure_equals("a position", getVariableDecl(tree->functions[0].stmts[0]).name.position, at(3, 9));
    ensure_equals("a name", getVariableDecl(tree->functions[0].stmts[0]).name.value, "a");
    ensure("a has value", getVariableDecl(tree->functions[0].stmts[0]).value);
    ensure_equals("b position", getIdentifier(getVariableDecl(tree->functions[0].stmts[0]).value->first).position, at(3, 13));
    ensure_equals("b name", getIdentifier(getVariableDecl(tree->functions[0].stmts[0]).value->first).value, "b");
}

template <>
template <>
void object::test<18>()
{
    using namespace rask;
    
    source << "f(int32 x, int32 y, int32 z) -> void\n{ }";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].name.value, "f");
    ENSURE_EQUALS(tree->functions[0].name.position, at(1, 1));
    ENSURE_EQUALS(tree->functions[0].args.size(), 3u);
    ENSURE_EQUALS(tree->functions[0].args[0].value, "x");
    ENSURE_EQUALS(tree->functions[0].args[0].position, at(1, 9));
    ENSURE_EQUALS(tree->functions[0].args[1].value, "y");
    ENSURE_EQUALS(tree->functions[0].args[1].position, at(1, 18));
    ENSURE_EQUALS(tree->functions[0].args[2].value, "z");
    ENSURE_EQUALS(tree->functions[0].args[2].position, at(1, 27));
    ENSURE(tree->functions[0].stmts.empty());
}

template <>
template <>
void object::test<19>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{ }";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].type.value, "abcd123");
    ENSURE_EQUALS(tree->functions[0].type.position, at(1, 8));
}

template <>
template <>
void object::test<20>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    return 10;\n    return x;\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].type.value, "abcd123");
    ENSURE_EQUALS(tree->functions[0].type.position, at(1, 8));
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 2u);
    ENSURE_EQUALS(getReturn(tree->functions[0].stmts[0]).position, at(3, 5));
    ENSURE_EQUALS(getConstant(getReturn(tree->functions[0].stmts[0]).value.first).value, 10);
    ENSURE_EQUALS(getConstant(getReturn(tree->functions[0].stmts[0]).value.first).position, at(3, 12));
    ENSURE_EQUALS(getReturn(tree->functions[0].stmts[1]).position, at(4, 5));
    ENSURE_EQUALS(getIdentifier(getReturn(tree->functions[0].stmts[1]).value.first).value, "x");
    ENSURE_EQUALS(getIdentifier(getReturn(tree->functions[0].stmts[1]).value.first).position, at(4, 12));
}

template <>
template <>
void object::test<21>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    var a = g(10, x);\n}";

    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    const cst::FunctionCall& fc = getFunctionCall(getVariableDecl(tree->functions[0].stmts[0]).value->first);
    ENSURE_EQUALS(fc.function.position, at(3, 13));
    ENSURE_EQUALS(fc.function.value, "g");
    ENSURE_EQUALS(fc.args.size(), 2u);
    ENSURE_EQUALS(getConstant(fc.args[0]).position, at(3, 15));
    ENSURE_EQUALS(getConstant(fc.args[0]).value, 10);
    ENSURE_EQUALS(getIdentifier(fc.args[1]).position, at(3, 19));
    ENSURE_EQUALS(getIdentifier(fc.args[1]).value, "x");
}

template <>
template <>
void object::test<22>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    h(g(5));\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    ENSURE_EQUALS(getFunctionCall(tree->functions[0].stmts[0]).args.size(), 1u);
    const cst::FunctionCall& fc = getFunctionCall(getFunctionCall(tree->functions[0].stmts[0]).args[0]);
    ENSURE_EQUALS(fc.function.position, at(3, 7));
    ENSURE_EQUALS(fc.function.value, "g");
    ENSURE_EQUALS(fc.args.size(), 1u);
    ENSURE_EQUALS(getConstant(fc.args[0]).position, at(3, 9));
    ENSURE_EQUALS(getConstant(fc.args[0]).value, 5);
}

template <>
template <>
void object::test<23>()
{
    using namespace rask;
    
    source << "f() -> abcd123\n{\n    return g();\n}";
    
    boost::optional<cst::Tree> tree = parseFile();
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    const cst::Return& r = getReturn(tree->functions[0].stmts[0]);
    ENSURE_EQUALS(r.position, at(3, 5));
    const cst::FunctionCall& fc = getFunctionCall(r.value.first);
    ENSURE(r.value.next.empty());
    ENSURE_EQUALS(fc.function.position, at(3, 12));
    ENSURE_EQUALS(fc.function.value, "g");
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
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    const cst::UnaryOperatorCall& e = getUnaryOperatorCall(getReturn(tree->functions[0].stmts[0]).value.first);
    ENSURE(e.op.tag == cst::UnaryOperator::MINUS);
    ENSURE_EQUALS(e.op.position, at(3, 12));
    const cst::Identifier& v = getIdentifier(e.expr);
    ENSURE_EQUALS(v.position, at(3, 13));
    ENSURE_EQUALS(v.value, "x");
}

template <>
template <>
void object::test<25>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    return x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    const cst::Expression& e = getReturn(tree->functions[0].stmts[0]).value;
    const cst::Identifier& x = getIdentifier(e.first);
    ENSURE_EQUALS(x.position, at(3, 12));
    ENSURE_EQUALS(x.value, "x");
    ENSURE_EQUALS(e.next.size(), 2u);
    ENSURE(e.next[0].op.tag == cst::BinaryOperator::PLUS);
    ENSURE_EQUALS(e.next[0].op.position, at(3, 14));
    const cst::Identifier& y = getIdentifier(e.next[0].expr);
    ENSURE_EQUALS(y.position, at(3, 16));
    ENSURE_EQUALS(y.value, "y");
    ENSURE(e.next[1].op.tag == cst::BinaryOperator::MINUS);
    ENSURE_EQUALS(e.next[1].op.position, at(3, 18));
    const cst::Constant& five = getConstant(e.next[1].expr);
    ENSURE_EQUALS(five.position, at(3, 20));
    ENSURE_EQUALS(five.value, 5);
}

template <>
template <>
void object::test<26>()
{
    using namespace rask;

    source << "f() -> abcd123\n{\n    var z = x + y - 5;\n}";

    boost::optional<cst::Tree> tree = parseFile();

    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 1u);
    ENSURE(getVariableDecl(tree->functions[0].stmts[0]).value);
    const cst::Expression& e = *getVariableDecl(tree->functions[0].stmts[0]).value;
    const cst::Identifier& x = getIdentifier(e.first);
    ENSURE_EQUALS(x.position, at(3, 13));
    ENSURE_EQUALS(x.value, "x");
    ENSURE_EQUALS(e.next.size(), 2u);
    ENSURE(e.next[0].op.tag == cst::BinaryOperator::PLUS);
    ENSURE_EQUALS(e.next[0].op.position, at(3, 15));
    const cst::Identifier& y = getIdentifier(e.next[0].expr);
    ENSURE_EQUALS(y.position, at(3, 17));
    ENSURE_EQUALS(y.value, "y");
    ENSURE(e.next[1].op.tag == cst::BinaryOperator::MINUS);
    ENSURE_EQUALS(e.next[1].op.position, at(3, 19));
    const cst::Constant& five = getConstant(e.next[1].expr);
    ENSURE_EQUALS(five.position, at(3, 21));
    ENSURE_EQUALS(five.value, 5);
}

}
