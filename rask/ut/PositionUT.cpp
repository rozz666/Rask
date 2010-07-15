#include <sstream>
#include <boost/lexical_cast.hpp>
#include <rask/Position.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

namespace tut
{

struct IdPosition_TestData
{
};

typedef test_group<IdPosition_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.Position");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    rask::Position p;

    ensure_equals("file", p.file, "");
    ensure_equals("row", p.row, 0u);
    ensure_equals("column", p.column, 0u);
}

template <>
template <>
void object::test<2>()
{
    rask::Position p("asia", 7, 19);

    ensure_equals("file", p.file, "asia");
    ensure_equals("row", p.row, 7u);
    ensure_equals("column", p.column, 19u);
}

template <>
template <> 
void object::test<3>()
{
    ensure("equal", rask::Position("abc", 123, 456) == rask::Position("abc", 123, 456));
    ensure_not("unequal 1", rask::Position("abc", 5123, 274) == rask::Position("abc", 123, 456));
    ensure_not("unequal 2", rask::Position("abc", 5123, 274) == rask::Position("abc", 123, 274));
    ensure_not("unequal 3", rask::Position("abc", 5123, 274) == rask::Position("abc", 5123, 456));
    ensure_not("unequal 4", rask::Position("abc", 123, 456) == rask::Position("xxx", 123, 456)); 
}


template <>
template <>
void object::test<4>()
{
    ensure_not("equal", rask::Position("abc", 123, 456) != rask::Position("abc", 123, 456));
    ensure("unequal 1", rask::Position("abc", 5123, 274) != rask::Position("abc", 123, 456));
    ensure("unequal 2", rask::Position("abc", 5123, 274) != rask::Position("abc", 123, 274));
    ensure("unequal 3", rask::Position("abc", 5123, 274) != rask::Position("abc", 5123, 456));
    ensure("unequal 4", rask::Position("abc", 123, 456) != rask::Position("xxx", 123, 456)); 
}

template <>
template <>
void object::test<5>()
{
    ensure_not("not less 1", rask::Position("abc", 123, 456) < rask::Position("abc", 123, 456));
    ensure_not("not less 2", rask::Position("abc", 123, 456) < rask::Position("abc", 123, 450));
    ensure_not("not less 3", rask::Position("abc", 123, 456) < rask::Position("abc", 120, 450));
    ensure_not("not less 4", rask::Position("zzz", 123, 456) < rask::Position("abc", 123, 456));
    ensure("less 1", rask::Position("abc", 123, 456) < rask::Position("abc", 123, 457));
    ensure("less 2", rask::Position("abc", 123, 456) < rask::Position("abc", 124, 457));
    ensure("less 3", rask::Position("abc", 123, 456) < rask::Position("zzz", 123, 456)); 
}

template <>
template <>
void object::test<6>()
{
    ensure_equals(boost::lexical_cast<std::string>(rask::Position("abc", 123, 456)), "abc(123, 456)"); 
    ensure_equals(boost::lexical_cast<std::string>(rask::Position()), "");
    ensure_equals(boost::lexical_cast<std::string>(rask::Position("abc", 100, 0)), "abc(100)"); 
    ensure_equals(boost::lexical_cast<std::string>(rask::Position("abc", 100)), "abc(100)"); 
    ensure_equals(boost::lexical_cast<std::string>(rask::Position("abc", 0, 0)), "abc"); 
    ensure_equals(boost::lexical_cast<std::string>(rask::Position("abc")), "abc"); 
}

template <>
template <>
void object::test<7>()
{
    rask::Position p("asia");

    ensure_equals("file", p.file, "asia");
    ensure_equals("row", p.row, 0u);
    ensure_equals("column", p.column, 0u);
}

template <>
template <>
void object::test<8>()
{
    rask::Position p("asia", 10);

    ensure_equals("file", p.file, "asia");
    ensure_equals("row", p.row, 10u);
    ensure_equals("column", p.column, 0u);
}

}
