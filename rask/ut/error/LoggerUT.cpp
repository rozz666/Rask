#include <rask/error/Logger.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

namespace tut
{

struct Logger_TestData
{
};

typedef test_group<Logger_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.error.Logger");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    rask::error::Logger logger;

    ensure("no errors", logger.errors().empty());
}

template <>
template <>
void object::test<2>()
{
    rask::error::Logger logger;
    rask::Position pos("xxx", 3, 4);

    logger.log(rask::error::Message(pos, "Asia"));

    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals("pos", logger.errors()[0].position(), pos);
    ensure_equals("text", logger.errors()[0].text(), "Asia");
}

}
