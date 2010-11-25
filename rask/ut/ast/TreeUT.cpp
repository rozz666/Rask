// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/ast/Tree.hpp>

namespace tut
{

struct Tree_TestData
{
};

typedef test_group<Tree_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Tree");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    ensure_equals("empty", ast::Tree().functionCount(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::Tree tree;
    ast::SharedCustomFunction f1(new ast::CustomFunction(cst::Identifier::create(Position(), "abc"), ast::VOID));
    ast::SharedCustomFunction f2(new ast::CustomFunction(cst::Identifier::create(Position(), "def"), ast::VOID));
    
    tree.add(f1);
    ensure("f1", tree.function(0) == f1);
    ensure_equals("count 1", tree.functionCount(), 1u);
    
    tree.add(f2);
    ensure("f2", tree.function(1) == f2);
    ensure_equals("count 2", tree.functionCount(), 2u);
}

}
