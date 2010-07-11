#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/cst/parseMain.hpp>

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
tut::factory tf("rask.cst.parseMain");
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

    boost::optional<cst::Function> f = cst::parseMain(source, errorLogger);

    ensure("parsed", f.is_initialized());
    ensure_equals("name", f->name.value, "main");
    ensure_equals("name pos", f->name.position, Position(source.file(), 1, 1));
    ensure(errorLogger.errors().empty());
}

}
