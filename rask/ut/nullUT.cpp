// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <rask/null.hpp>

TEST(rask_null, pointerInitialization)
{
    void *v = rask::null;
    char *c = rask::null;
    boost::shared_ptr<float> s = rask::null;
    boost::weak_ptr<double> w = rask::null;

    ASSERT_TRUE(v == 0);
    ASSERT_TRUE(c == 0);
    ASSERT_TRUE(s == boost::shared_ptr<float>());
    ASSERT_TRUE(w.expired());
}

TEST(rask_null, pointerEquality)
{
    int *p1 = 0;
    int x;
    int *p2 = &x;

    ASSERT_TRUE(rask::null == p1);
    ASSERT_TRUE(p1 == rask::null);
    ASSERT_TRUE(!(rask::null != p1));
    ASSERT_TRUE(!(p1 != rask::null));

    ASSERT_TRUE(rask::null != p2);
    ASSERT_TRUE(p2 != rask::null);
    ASSERT_TRUE(!(rask::null == p2));
    ASSERT_TRUE(!(p2 == rask::null));

    ASSERT_TRUE(rask::null == rask::null);
    ASSERT_TRUE(!(rask::null != rask::null));
}

TEST(rask_null, optionalInitialization)
{
    boost::optional<int> x = rask::null;
    ASSERT_FALSE(x);
}
