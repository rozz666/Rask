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
#include <rask/cg/SymbolTable.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>

namespace tut
{

struct SymbolTable_TestData
{
    llvm::LLVMContext ctx;
    llvm::AllocaInst *a1;
    llvm::AllocaInst *a2;
    rask::cg::SymbolTable st;
    
    SymbolTable_TestData()
        : a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32))),
        a2(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 64))) { }
};

typedef test_group<SymbolTable_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.SymbolTable");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Identifier id1 = cst::Identifier::create(Position(), "x");
    cst::Identifier id2 = cst::Identifier::create(Position(), "y");
    st.add(id1, a1);
    st.add(id2, a2);
    
    ensure(st.get(id1) == a1);
    ensure(st.get(id2) == a2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cst::Identifier id1 = cst::Identifier::create(Position(), "x");
   
    st.add(id1, a1);

    try
    {
        st.add(id1, a2);
        fail("expected SymbolTableError");
    }
    catch (const cg::SymbolTableError& e)
    {
        ensure_equals(e.what(), std::string("Duplicated identifier"));
    }
    
    ensure(st.get(id1) == a1);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cst::Identifier id1 = cst::Identifier::create(Position(), "x");
    
    try
    {
        st.get(id1);
        fail("expected SymbolTableError");
    }
    catch (const cg::SymbolTableError& e)
    {
        ensure_equals(e.what(), std::string("Symbol \'x\' not found"));
    }
}

}
