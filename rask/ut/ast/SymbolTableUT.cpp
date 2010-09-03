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
#include <rask/ast/SymbolTable.hpp>

namespace tut
{

struct AstSymbolTable_TestData
{
    rask::ast::SymbolTable st;
};

typedef test_group<AstSymbolTable_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.SymbolTable");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::SharedVariable var1(new ast::Variable(cst::Identifier::create(Position(), "asia")));
    ast::SharedVariable var2(new ast::Variable(cst::Identifier::create(Position(), "kasia")));

    st.add(var1);
    st.add(var2);
    
    ensure("var1", st.get(var1->name()) == var1);
    ensure("var2", st.get(var2->name()) == var2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cst::Identifier id1 = cst::Identifier::create(Position(), "x");
    
    try
    {
        st.get(id1);
        fail("expected SymbolTableError");
    }
    catch (const ast::SymbolTableError& e)
    {
        ensure_equals(e.what(), std::string("Symbol \'x\' not found"));
    }
}

}
