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

    const rask::cst::FunctionCall& getFCall(const rask::cst::Statement& stmt)
    {
        return boost::get<rask::cst::FunctionCall>(stmt);
    }

    const rask::cst::VarDecl& getVDecl(const rask::cst::Statement& stmt)
    {
        return boost::get<rask::cst::VarDecl>(stmt);
    }

    const rask::cst::Identifier& getId(const rask::cst::Expression& expr)
    {
        return boost::get<rask::cst::Identifier>(expr);
    }

    const rask::cst::Constant& getConst(const rask::cst::Expression& expr)
    {
        return boost::get<rask::cst::Constant>(expr);
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
    ensure_equals("name", tree->main.name.value, "main");
    ensure_equals("name pos", tree->main.name.position, Position(source.file(), 1, 1));
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
    ensure_equals("name", tree->main.name.value, "main");
    ensure_equals("name pos", tree->main.name.position, Position(source.file(), 1, 1));
    ensure_equals("count", tree->main.stmts.size(), 3u);

    ensure_equals("function 1", getFCall(tree->main.stmts[0]).function.value, "abcd");
    ensure_equals("function pos 1", getFCall(tree->main.stmts[0]).function.position, Position(source.file(), 3, 5));
    ensure_equals("count 1", getFCall(tree->main.stmts[0]).args.size(), 0u);

    ensure_equals("function 2", getFCall(tree->main.stmts[1]).function.value, "efgh");
    ensure_equals("function pos 2", getFCall(tree->main.stmts[1]).function.position, Position(source.file(), 4, 5));
    ensure_equals("count 2", getFCall(tree->main.stmts[1]).args.size(), 1u);
    ensure_equals("value 2", getConst(getFCall(tree->main.stmts[1]).args[0]).value, -2);
    ensure_equals("value 2 pos", getConst(getFCall(tree->main.stmts[1]).args[0]).position, Position(source.file(), 4, 10));
    
    ensure_equals("function 3", getFCall(tree->main.stmts[2]).function.value, "ijkl");
    ensure_equals("function pos 3", getFCall(tree->main.stmts[2]).function.position, Position(source.file(), 5, 5));
    ensure_equals("count 3", getFCall(tree->main.stmts[2]).args.size(), 2u);
    ensure_equals("value 3", getConst(getFCall(tree->main.stmts[2]).args[0]).value, 2);
    ensure_equals("value 3 pos", getConst(getFCall(tree->main.stmts[2]).args[0]).position, Position(source.file(), 5, 10));
    ensure_equals("value 4", getConst(getFCall(tree->main.stmts[2]).args[1]).value, 3);
    ensure_equals("value 4 pos", getConst(getFCall(tree->main.stmts[2]).args[1]).position, Position(source.file(), 5, 13));
    ensureNoErrors();
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

    ensure_equals("statements", tree->main.stmts.size(), 2u);

    ensure_equals("x position", getVDecl(tree->main.stmts[0]).name.position, Position(source.file(), 3, 9));
    ensure_equals("x name", getVDecl(tree->main.stmts[0]).name.value, "x");
    ensure_not("no x value", getVDecl(tree->main.stmts[0]).value);

    ensure_equals("y position", getVDecl(tree->main.stmts[1]).name.position, Position(source.file(), 4, 9));
    ensure_equals("y name", getVDecl(tree->main.stmts[1]).name.value, "y");
    ensure("y has value", getVDecl(tree->main.stmts[1]).value);
    ensure_equals("y value", getVDecl(tree->main.stmts[1]).value->value, -2);
    ensure_equals("y value position", getVDecl(tree->main.stmts[1]).value->position, Position(source.file(), 4, 13));
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

    ensure_size("statements", tree->main.stmts, 1u);

    ensure_equals("name", getFCall(tree->main.stmts[0]).function.value, "f");
    ensure_equals("func pos", getFCall(tree->main.stmts[0]).function.position, Position(source.file(), 3, 5));
    ensure_size("args", getFCall(tree->main.stmts[0]).args, 3u);
    ensure_equals(getId(getFCall(tree->main.stmts[0]).args[0]).value, "x");
    ensure_equals(getId(getFCall(tree->main.stmts[0]).args[0]).position, Position(source.file(), 3, 7));
    ensure_equals(getConst(getFCall(tree->main.stmts[0]).args[1]).value, 1);
    ensure_equals(getConst(getFCall(tree->main.stmts[0]).args[1]).position, Position(source.file(), 3, 10));
    ensure_equals(getId(getFCall(tree->main.stmts[0]).args[2]).value, "y");
    ensure_equals(getId(getFCall(tree->main.stmts[0]).args[2]).position, Position(source.file(), 3, 13));
}

}
