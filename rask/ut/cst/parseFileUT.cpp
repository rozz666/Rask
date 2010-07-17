#include <algorithm>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure("parsed", f.is_initialized());
    ensure_equals("name", f->name.value, "main");
    ensure_equals("name pos", f->name.position, Position(source.file(), 1, 1));
    ensureNoErrors();
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    InputStream source("test.rask", ss);

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
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

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
    ensureErrorCountEquals(1);
    ensureError(error::Message::missingRightArrow(Position(source.file(), 1, 8)));
}

}
