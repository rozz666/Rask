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
#include <rask/cst/Optimizer.hpp>

namespace tut
{

struct Optimizer_TestData
{
    rask::cst::Optimizer optimizer;
};

typedef test_group<Optimizer_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cst.Optimizer");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    int c = 8, c2 = 13;

    cst::ChainExpression ce;
    ce.expr = cst::Constant::create(Position(), c);
    ce.next.resize(1);
    ce.next[0].expr = cst::Constant::create(Position(), c2);

    cst::ChainExpression ce2;
    ce2.expr = ce;
    cst::ChainExpression ce3;
    ce3.expr = ce2;

    optimizer.optimize(ce3);
    ENSURE_EQUALS(getConstant(ce3.expr).value, c);
    ENSURE_EQUALS(ce3.next.size(), 1u);
    ENSURE_EQUALS(getConstant(ce3.next[0].expr).value, c2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    int c = 7;

    cst::ChainExpression ce;
    ce.expr = cst::Constant::create(Position(), c);

    optimizer.optimize(ce);
    ENSURE_EQUALS(getConstant(ce.expr).value, c);
    ENSURE_EQUALS(ce.next.size(), 0u);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    int c = 8, c2 = 13;

    cst::ChainExpression ce1, ce2, ce3, ce4, ce5;
    ce1.expr = cst::Constant::create(Position(), c);
    ce2.expr = ce1;

    ce3.expr = ce2;
    ce3.next.resize(1);
    ce3.next[0].expr = cst::Constant::create(Position(), c2);

    ce4.expr = ce3;
    ce5.expr = ce4;

    optimizer.optimize(ce5);
    ENSURE_EQUALS(getConstant(ce5.expr).value, c);
    ENSURE_EQUALS(ce5.next.size(), 1u);
    ENSURE_EQUALS(getConstant(ce5.next[0].expr).value, c2);
}

}
