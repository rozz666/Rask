#include <rask/error/Message.hpp>
#include <boost/lexical_cast.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

namespace tut
{

struct Message_TestData
{
    rask::Position pos;
    
    Message_TestData() : pos("sample.rask", 1, 2) { }
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
    ensure_equals(boost::lexical_cast<std::string>(rask::error::Message(pos, "xxx")), boost::lexical_cast<std::string>(pos) + ": error: xxx");
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    ensure_equals(error::Message::missingMainFunction(pos), error::Message(pos, "missing main function"));
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    ensure_equals(error::Message::missingReturnType(pos), error::Message(pos, "missing return type"));
}

template <>
template <>
void object::test<7>()
{
    using namespace rask;
    ensure_equals(error::Message::missingOpeningBrace(pos), error::Message(pos, "missing opening brace '{'"));
}

template <>
template <>
void object::test<8>()
{
    using namespace rask;
    ensure_equals(error::Message::missingClosingBrace(pos), error::Message(pos, "missing closing brace '}'"));
}

template <>
template <>
void object::test<9>()
{
    using namespace rask;
    ensure_equals(error::Message::missingOpeningParen(pos), error::Message(pos, "missing opening parenthesis '('"));
}

template <>
template <>
void object::test<10>()
{
    using namespace rask;
    ensure_equals(error::Message::missingClosingParen(pos), error::Message(pos, "missing closing parenthesis ')'"));
}

template <>
template <>
void object::test<11>()
{
    using namespace rask;
    ensure_equals(error::Message::missingRightArrow(pos), error::Message(pos, "missing '->'"));
}


}
