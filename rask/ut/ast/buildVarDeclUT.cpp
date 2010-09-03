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
#include <rask/ast/Builder.hpp>

namespace tut
{

struct buildVarDecl_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    rask::ast::Builder builder;

    buildVarDecl_TestData() : builder(logger, st) { }
};

typedef test_group<buildVarDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildVarDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::VarDecl cvd;
    cvd.name = cst::Identifier::create(Position(), "x");
    cvd.value = cst::Constant::create(Position(), 1);

    boost::optional<ast::VarDecl> vd = builder.buildVarDecl(cvd);

    ensure("built", vd);
    ensure("no errors", logger.errors().empty());
    ensure_equals("name pos", vd->var()->name().position, cvd.name.position);
    ensure_equals("name str", vd->var()->name().value, cvd.name.value);
    ensure_equals("value", vd->value(), cvd.value->value);
    ensure("st", st.get(cvd.name) == vd->var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::VarDecl cvd;
    cvd.name = cst::Identifier::create(Position(), "x");
    
    boost::optional<ast::VarDecl> vd = builder.buildVarDecl(cvd);
    
    ensure_not("not built", vd);
    ensure_equals("1 error", logger.errors().size(), 1u);
    
}

}
