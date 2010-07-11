#!/usr/bin/bash
if [ ! "$#" = "2" ]; then
    echo "usage: make_ut.sh <name> <desc>"
    exit 1
fi
NAME="$1"
DESC="$2"
CODE="#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

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
