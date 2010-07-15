#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/cst/parseFile.hpp>

namespace tut
{

struct parseMain_TestData
{
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

    std::stringstream ss;

    ss << "main() -> void\n{\n}";

    InputStream source("test.rask", ss);
    error::Logger errorLogger;

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure("parsed", f.is_initialized());
    ensure_equals("name", f->name.value, "main");
    ensure_equals("name pos", f->name.position, Position(source.file(), 1, 1));
    ensure("no errors", errorLogger.errors().empty());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    std::stringstream ss;

    InputStream source("test.rask", ss);
    error::Logger errorLogger;

    boost::optional<cst::Function> f = cst::parseFile(source, errorLogger);

    ensure_not("not parsed", f.is_initialized());
    ensure_equals("errors", errorLogger.errors().size(), 1u);
    ensure_equals("message", errorLogger.errors()[0], error::Message::missingMainFunction(Position(source.file())));
}

}
