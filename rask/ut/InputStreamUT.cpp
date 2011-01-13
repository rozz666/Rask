// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <rask/InputStream.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>

namespace tut
{

struct InputStream_TestData
{
};

typedef test_group<InputStream_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.InputStream");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    std::string text = "Asia abc";
    std::stringstream ss(text);
    rask::InputStream is("xxx", ss);

    ensure_equals("file", is.file(), "xxx");
    ensure_equals("source", std::string(is.begin(), is.end()), text);
}


}
