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

    ensure_equals(ast::Function().stmtCount(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ast::Function f;
    ast::FunctionCall fc(5);
    cst::Identifier name;
    name.value = "asia";
    ast::SharedVarDecl vd(new ast::VarDecl(name, 0));
    f.addStmt(fc);
    f.addStmt(vd);
    ensure_equals("count", f.stmtCount(), 2u);
    ensure_equals("call", boost::get<ast::FunctionCall>(f.stmt(0)), fc);
    ensure_equals("name", boost::get<ast::SharedVarDecl>(f.stmt(1))->name().value, name.value);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    ast::Function f1;
    ast::Function f2;

    ensure("eq 1", f1 == f2);
    ensure_not("neq 1", f1 != f2);

    f1.addStmt(ast::FunctionCall(1));

    ensure_not("eq 2", f1 == f2);
    ensure("neq 2", f1 != f2);

    f2.addStmt(ast::FunctionCall(1));

    ensure("eq 3", f1 == f2);
    ensure_not("neq 3", f1 != f2);
}
    
}
