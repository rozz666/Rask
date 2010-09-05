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
#include <rask/test/TUTAssert.hpp>
#include <rask/ast/Function.hpp>

namespace tut
{

struct Function_TestData
{
};

typedef test_group<Function_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Function");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    cst::Identifier name = cst::Identifier::create(Position("abc", 10, 20), "kasia");
    ast::Function f(name);
    ast::FunctionCall fc(5);
    ast::VariableDecl vd(cst::Identifier::create(Position("xx", 1, 2), "asia"), 0);
    ensure_equals("no stmts", f.stmtCount(), 0u);
    f.addStmt(fc);
    f.addStmt(vd);
    ensure_equals("name pos", f.name().position, name.position);
    ensure_equals("name val", f.name().value, name.value);
    ensure_equals("count", f.stmtCount(), 2u);
    ensure_equals("call", boost::get<boost::int32_t>(boost::get<ast::FunctionCall>(f.stmt(0))), boost::get<boost::int32_t>(fc));
    ensure("var", boost::get<ast::VariableDecl>(f.stmt(1)).var() == vd.var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ast::Function f1(cst::Identifier::create(Position("xxx", 1, 2), "aaa"));
    ast::Function f2(f1.name());
    ast::Function f3(cst::Identifier::create(f1.name().position, "bbb"));
    ast::Function f4(cst::Identifier::create(Position("xxx", 3, 4), f2.name().value));

    ENSURE(f1 == f2);
    ENSURE(!(f1 != f2));
    ENSURE(!(f1 == f3));
    ENSURE(f1 != f3);
    ENSURE(f3 != f4);
    ENSURE(!(f3 == f4));
    
    f1.addStmt(ast::FunctionCall(1));

    ENSURE(!(f1 == f2));
    ENSURE(f1 != f2);

    f2.addStmt(ast::FunctionCall(1));

    ENSURE(f1 == f2);
    ENSURE(!(f1 != f2));

    cst::Identifier name;
    name.value = "asia";
    ast::VariableDecl vd(name, 1);
    f1.addStmt(vd);
    
    ENSURE(!(f1 == f2));
    ENSURE(f1 != f2);
    
    f2.addStmt(vd);
    
    ENSURE(f1 == f2);
    ENSURE(!(f1 != f2));
}
    
}
