// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/ast/Constant.hpp>

namespace tut
{

struct Constant_TestData
{
};

typedef test_group<Constant_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Constant");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    boost::int32_t n = 35;
    ast::Constant c1(boost::int32_t(77));
    ast::Constant c2(n);
    ast::Constant c3(n);

    ENSURE(c1 != c2);
    ENSURE(!(c1 == c2));
    ENSURE(c2 == c3);
    ENSURE(!(c2 != c3));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    boost::int32_t n1 = 77;
    boost::int32_t n2 = 35;
    ast::Constant c1(n1);
    ast::Constant c2(n2);

    ENSURE_EQUALS(c1.getInt32(), n1);
    ENSURE_EQUALS(c2.getInt32(), n2);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    ast::Constant c1(true);
    ast::Constant c2(false);
    ast::Constant c3(false);

    ENSURE(c1 != c2);
    ENSURE(!(c1 == c2));
    ENSURE(c2 == c3);
    ENSURE(!(c2 != c3));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    bool v1 = true;
    bool v2 = false;
    ast::Constant c1(v1);
    ast::Constant c2(v2);

    ENSURE(c1.getBoolean() == v1);
    ENSURE(c2.getBoolean() == v2);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    ENSURE(ast::Constant(false) != ast::Constant(boost::int32_t(0)));
    ENSURE(!(ast::Constant(false) == ast::Constant(boost::int32_t(0))));
    ENSURE(ast::Constant(boost::int32_t(0)) != ast::Constant(false));
    ENSURE(!(ast::Constant(boost::int32_t(0)) == ast::Constant(false)));
}

}
