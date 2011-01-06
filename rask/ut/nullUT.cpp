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
#include <rask/null.hpp>
#include <boost/concept_check.hpp>

namespace tut
{

struct null_TestData
{
};

typedef test_group<null_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.null");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    void *v = rask::null;
    char *c = rask::null;
    boost::shared_ptr<float> s = rask::null;
    boost::weak_ptr<double> w = rask::null;

    ENSURE(v == 0);
    ENSURE(c == 0);
    ENSURE(s == boost::shared_ptr<float>());
    ENSURE(w.expired());
}

template <>
template <>
void object::test<2>()
{
    int *p1 = 0;
    int x;
    int *p2 = &x;

    ENSURE(rask::null == p1);
    ENSURE(p1 == rask::null);
    ENSURE(!(rask::null != p1));
    ENSURE(!(p1 != rask::null));

    ENSURE(rask::null != p2);
    ENSURE(p2 != rask::null);
    ENSURE(!(rask::null == p2));
    ENSURE(!(p2 == rask::null));

    ENSURE(rask::null == rask::null);
    ENSURE(!(rask::null != rask::null));
}

}
