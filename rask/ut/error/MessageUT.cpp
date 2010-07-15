#include <rask/error/Message.hpp>
#include <boost/lexical_cast.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

namespace tut
{

struct Message_TestData
{
};

typedef test_group<Message_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.error.Message");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    const rask::Position pos("abc", 123, 456);
    const std::string text = "This is an error";
    rask::error::Message msg(pos, text);

    ensure_equals("pos", msg.position(), pos);
    ensure_equals("text", msg.text(), text);
}

template <>
template <>
void object::test<3>()
{
    rask::error::Message msg1(rask::Position("asia", 1, 2), "error");
    rask::error::Message msg2(rask::Position("kasia", 1, 2), "error");
    rask::error::Message msg3(rask::Position("asia", 1, 2), "xxx");

    ensure("1", msg1 == msg1);
    ensure_not("2", msg1 != msg1);
    ensure("3", msg1 != msg2);
    ensure_not("4", msg1 == msg2);
    ensure("5", msg1 != msg3);
    ensure_not("6", msg1 == msg3);
}

template <>
template <>
void object::test<4>()
{
    rask::error::Message msg1(rask::Position("asia", 1, 2), "xxx");

    std::ostringstream os;

    os << msg1;

    ensure_equals(os.str(), "asia(1, 2): error: xxx");
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    ensure_equals(error::Message::missingMainFunction(Position("sample.rask")), error::Message(Position("sample.rask"), "missing main function"));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    ensure_equals(error::Message::missingReturnType(Position("sample.rask", 1, 2)), error::Message(Position("sample.rask", 1, 2), "missing return type"));
}

}
