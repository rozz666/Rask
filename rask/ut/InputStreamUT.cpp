// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <gtest/gtest.h>
#include <sstream>
#include <rask/InputStream.hpp>

TEST(rask_InputStream, iterators)
{
    std::string text = "Asia abc";
    std::string file = "xxx";
    std::stringstream ss(text);
    rask::InputStream is(file, ss);

    ASSERT_EQ(file, is.file());
    ASSERT_EQ(text, std::string(is.begin(), is.end()));
}

