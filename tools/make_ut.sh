#!/bin/bash
# Rask
#
# Copyright (c) 2010 Rafal Przywarski
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#

if [ ! "$#" = "2" ]; then
    echo "usage: make_ut.sh <name> <desc>"
    exit 1
fi
NAME="$1"
DESC="$2"
HEADER=${DESC//"."/"/"}
CODE="// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <${HEADER}.hpp>

namespace tut
{

struct ${NAME}_TestData
{
};

typedef test_group<${NAME}_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf(\"$DESC\");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
}

template <>
template <>
void object::test<2>()
{
}

template <>
template <>
void object::test<3>()
{
}

}"

echo "$CODE" > "${NAME}UT.cpp"
