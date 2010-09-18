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

struct buildVariableDecl_TestData
{
    rask::error::Logger logger;
    rask::ast::SymbolTable st;
    rask::ast::Builder builder;

    buildVariableDecl_TestData() : builder(logger, st) { }
};

typedef test_group<buildVariableDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildVariableDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::VariableDecl cvd;
    cvd.name = cst::Identifier::create(Position(), "x");
    cvd.value = cst::Constant::create(Position(), 1);

    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd);

    ensure("built", vd);
    ensure("no errors", logger.errors().empty());
    ensure_equals("name pos", vd->var()->name().position, cvd.name.position);
    ensure_equals("name str", vd->var()->name().value, cvd.name.value);
    ensure_equals("value", getConstant(vd->value()), getConstant(*cvd.value).value);
    ensure("st", st.getVariable(cvd.name.value) == vd->var());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst::VariableDecl cvd;
    cvd.name = cst::Identifier::create(Position(), "x");
    
    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd);
    
    ensure_not("not built", vd);
    ensure_equals("1 error", logger.errors().size(), 1u);
    
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ast::SharedVariable y(new ast::Variable(cst::Identifier::create(Position(), "y")));
    st.add(y);
    
    cst::VariableDecl cvd;
    cvd.name = cst::Identifier::create(Position(), "x");
    cvd.value = cst::Identifier::create(Position(), y->name().value);
    
    boost::optional<ast::VariableDecl> vd = builder.buildVariableDecl(cvd);
    
    ensure("built", vd);
    ensure("no errors", logger.errors().empty());
    ensure_equals("name pos", vd->var()->name().position, cvd.name.position);
    ensure_equals("name str", vd->var()->name().value, cvd.name.value);
    ensure_equals("y pos", getVariable(vd->value()).lock()->name().position, getIdentifier(*cvd.value).position);
    ensure_equals("y str", getVariable(vd->value()).lock()->name().value, getIdentifier(*cvd.value).value);
    ensure("st", st.getVariable(cvd.name.value) == vd->var());
}

}
