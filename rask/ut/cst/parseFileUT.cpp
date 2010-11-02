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
    std::stringstream ss;
    rask::error::Logger errorLogger;

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

    ss << "main() -> void\n{\n}";

    InputStream source("test.rask", ss);

    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);

    ensure("parsed", tree);
    ensure_size("functions", tree->functions, 1u);
    ensure_equals("name", tree->functions[0].name.value, "main");
    ensure_equals("name pos", tree->functions[0].name.position, Position(source.file(), 1, 1));
    ensureNoErrors();
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingMainFunction(Position(source.file())));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ss << "main() -> \n{\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingReturnType(Position(source.file(), 2, 1)));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    ss << "main() -> void\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningBrace(Position(source.file(), 2, 1)));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    ss << "main() -> void\n{\n";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingBrace(Position(source.file(), 3, 1)));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    ss << "main) -> void\n{\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(Position(source.file(), 1, 5)));
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;

    ss << "main( -> void\n{\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingParen(Position(source.file(), 1, 7)));
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;

    ss << "main() - void\n{\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(Position(source.file(), 1, 8)));
}

template <>
template <>
void object::test<9>()
{
    using namespace rask;

    ss << "main() > void\n{\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(Position(source.file(), 1, 8)));
}

template <>
template <>
void object::test<10>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    abcd();\n    efgh(-2);\n    ijkl(2, 3);\n}";

    InputStream source("test.rask", ss);

    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);

    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    ensure_equals("name", tree->functions[0].name.value, "main");
    ensure_equals("name pos", tree->functions[0].name.position, Position(source.file(), 1, 1));
    ensure_equals("count", tree->functions[0].stmts.size(), 3u);

    ensure_equals("function 1", getFunctionCall(tree->functions[0].stmts[0]).function.value, "abcd");
    ensure_equals("function pos 1", getFunctionCall(tree->functions[0].stmts[0]).function.position, Position(source.file(), 3, 5));
    ensure_equals("count 1", getFunctionCall(tree->functions[0].stmts[0]).args.size(), 0u);

    ensure_equals("function 2", getFunctionCall(tree->functions[0].stmts[1]).function.value, "efgh");
    ensure_equals("function pos 2", getFunctionCall(tree->functions[0].stmts[1]).function.position, Position(source.file(), 4, 5));
    ensure_equals("count 2", getFunctionCall(tree->functions[0].stmts[1]).args.size(), 1u);
    ensure_equals("value 2", getConstant(getFunctionCall(tree->functions[0].stmts[1]).args[0]).value, -2);
    ensure_equals("value 2 pos", getConstant(getFunctionCall(tree->functions[0].stmts[1]).args[0]).position, Position(source.file(), 4, 10));
    
    ensure_equals("function 3", getFunctionCall(tree->functions[0].stmts[2]).function.value, "ijkl");
    ensure_equals("function pos 3", getFunctionCall(tree->functions[0].stmts[2]).function.position, Position(source.file(), 5, 5));
    ensure_equals("count 3", getFunctionCall(tree->functions[0].stmts[2]).args.size(), 2u);
    ensure_equals("value 3", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[0]).value, 2);
    ensure_equals("value 3 pos", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[0]).position, Position(source.file(), 5, 10));
    ensure_equals("value 4", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[1]).value, 3);
    ensure_equals("value 4 pos", getConstant(getFunctionCall(tree->functions[0].stmts[2]).args[1]).position, Position(source.file(), 5, 13));
}

template <>
template <>
void object::test<11>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    print(1)\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingSemicolon(Position(source.file(), 4, 1)));
}

template <>
template <>
void object::test<12>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    print;\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingOpeningParen(Position(source.file(), 3, 10)));
}

template <>
template <>
void object::test<13>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    print(5 5;\n}";

    InputStream source("test.rask", ss);

    ensure_not("not parsed", cst::parseFile(source, errorLogger));
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingClosingParen(Position(source.file(), 3, 13)));
}

template <>
template <>
void object::test<14>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    var x;\n    var y = -2;\n}";

    InputStream source("test.rask", ss);

    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_equals("statements", tree->functions[0].stmts.size(), 2u);

    ensure_equals("x position", getVariableDecl(tree->functions[0].stmts[0]).name.position, Position(source.file(), 3, 9));
    ensure_equals("x name", getVariableDecl(tree->functions[0].stmts[0]).name.value, "x");
    ensure_not("no x value", getVariableDecl(tree->functions[0].stmts[0]).value);

    ensure_equals("y position", getVariableDecl(tree->functions[0].stmts[1]).name.position, Position(source.file(), 4, 9));
    ensure_equals("y name", getVariableDecl(tree->functions[0].stmts[1]).name.value, "y");
    ensure("y has value", getVariableDecl(tree->functions[0].stmts[1]).value);
    ensure_equals("y value", getConstant(*getVariableDecl(tree->functions[0].stmts[1]).value).value, -2);
    ensure_equals("y value position", getConstant(*getVariableDecl(tree->functions[0].stmts[1]).value).position, Position(source.file(), 4, 13));
}

template <>
template <>
void object::test<15>()
{
    using namespace rask;

    ss << "main() -> void\n{\n    f(x, 1, y);\n}";

    InputStream source("test.rask", ss);

    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);

    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_size("statements", tree->functions[0].stmts, 1u);
    ensure_equals("name", getFunctionCall(tree->functions[0].stmts[0]).function.value, "f");
    ensure_equals("func pos", getFunctionCall(tree->functions[0].stmts[0]).function.position, Position(source.file(), 3, 5));
    ensure_size("args", getFunctionCall(tree->functions[0].stmts[0]).args, 3u);
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[0]).value, "x");
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[0]).position, Position(source.file(), 3, 7));
    ensure_equals(getConstant(getFunctionCall(tree->functions[0].stmts[0]).args[1]).value, 1);
    ensure_equals(getConstant(getFunctionCall(tree->functions[0].stmts[0]).args[1]).position, Position(source.file(), 3, 10));
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[2]).value, "y");
    ensure_equals(getIdentifier(getFunctionCall(tree->functions[0].stmts[0]).args[2]).position, Position(source.file(), 3, 13));
}

template <>
template <>
void object::test<16>()
{
    using namespace rask;
    
    ss << "f1() -> void\n{\n}\nf2() -> void\n{\n}\n";
    
    InputStream source("test.rask", ss);
    
    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 2u);

    ensure_equals(tree->functions[0].name.value, "f1");
    ensure_equals(tree->functions[0].name.position, Position(source.file(), 1, 1));
    ensure_empty("f1 no stmts", tree->functions[0].stmts);

    ensure_equals(tree->functions[1].name.value, "f2");
    ensure_equals(tree->functions[1].name.position, Position(source.file(), 4, 1));
    ensure_empty("f2 no stmts", tree->functions[1].stmts);
}

template <>
template <>
void object::test<17>()
{
    using namespace rask;
    
    ss << "main() -> void\n{\n    var a = b;\n}";
    
    InputStream source("test.rask", ss);
    
    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ensure("parsed", tree);
    ensureNoErrors();
    ensure_size("functions", tree->functions, 1u);
    
    ensure_equals("statements", tree->functions[0].stmts.size(), 1u);
   
    ensure_equals("a position", getVariableDecl(tree->functions[0].stmts[0]).name.position, Position(source.file(), 3, 9));
    ensure_equals("a name", getVariableDecl(tree->functions[0].stmts[0]).name.value, "a");
    ensure("a has value", getVariableDecl(tree->functions[0].stmts[0]).value);
    ensure_equals("b position", getIdentifier(*getVariableDecl(tree->functions[0].stmts[0]).value).position, Position(source.file(), 3, 13));
    ensure_equals("b name", getIdentifier(*getVariableDecl(tree->functions[0].stmts[0]).value).value, "b");
}

template <>
template <>
void object::test<18>()
{
    using namespace rask;
    
    ss << "f(int32 x, int32 y, int32 z) -> void\n{ }";
    
    InputStream source("test.rask", ss);
    
    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].name.value, "f");
    ENSURE_EQUALS(tree->functions[0].name.position, Position(source.file(), 1, 1));
    ENSURE_EQUALS(tree->functions[0].args.size(), 3u);
    ENSURE_EQUALS(tree->functions[0].args[0].value, "x");
    ENSURE_EQUALS(tree->functions[0].args[0].position, Position(source.file(), 1, 9));
    ENSURE_EQUALS(tree->functions[0].args[1].value, "y");
    ENSURE_EQUALS(tree->functions[0].args[1].position, Position(source.file(), 1, 18));
    ENSURE_EQUALS(tree->functions[0].args[2].value, "z");
    ENSURE_EQUALS(tree->functions[0].args[2].position, Position(source.file(), 1, 27));
    ENSURE(tree->functions[0].stmts.empty());
}

template <>
template <>
void object::test<19>()
{
    using namespace rask;
    
    ss << "f() -> abcd123\n{ }";
    
    InputStream source("test.rask", ss);
    
    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].type.value, "abcd123");
    ENSURE_EQUALS(tree->functions[0].type.position, Position(source.file(), 1, 8));
}

template <>
template <>
void object::test<20>()
{
    using namespace rask;
    
    ss << "f() -> abcd123\n{\n    return 10;\n    return x;\n}";
    
    InputStream source("test.rask", ss);
    
    boost::optional<cst::Tree> tree = cst::parseFile(source, errorLogger);
    
    ENSURE(tree);
    ensureNoErrors();
    ENSURE_EQUALS(tree->functions.size(), 1u);
    
    ENSURE_EQUALS(tree->functions[0].type.value, "abcd123");
    ENSURE_EQUALS(tree->functions[0].type.position, Position(source.file(), 1, 8));
    ENSURE_EQUALS(tree->functions[0].stmts.size(), 2u);
    ENSURE_EQUALS(getReturn(tree->functions[0].stmts[0]).position, Position(source.file(), 3, 5));
    ENSURE_EQUALS(getConstant(getReturn(tree->functions[0].stmts[0]).value).value, 10);
    ENSURE_EQUALS(getConstant(getReturn(tree->functions[0].stmts[0]).value).position, Position(source.file(), 3, 12));
    ENSURE_EQUALS(getReturn(tree->functions[0].stmts[1]).position, Position(source.file(), 4, 5));
    ENSURE_EQUALS(getIdentifier(getReturn(tree->functions[0].stmts[1]).value).value, "x");
    ENSURE_EQUALS(getIdentifier(getReturn(tree->functions[0].stmts[1]).value).position, Position(source.file(), 4, 12));
}

}
