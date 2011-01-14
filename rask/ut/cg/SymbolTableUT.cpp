// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/SymbolTable.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <gtest/gtest.h>

using namespace rask;

struct rask_cg_SymbolTable : testing::Test
{
    llvm::LLVMContext ctx;
    llvm::AllocaInst *val1;
    llvm::AllocaInst *val2;
    llvm::AllocaInst *val3;
    cg::SymbolTable table;
    cst::Identifier id1;

    rask_cg_SymbolTable()
        : val1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32))),
        val2(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 64))),
        val3(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 128))),
        id1(cst::Identifier::create(Position(), "x")) { }
};

TEST_F(rask_cg_SymbolTable, addThreeSymbols)
{
    cst::Identifier id2 = cst::Identifier::create(Position(), "y");
    cst::Identifier id3 = cst::Identifier::create(Position("", 2, 3), "y");
    table.add(id1, val1);
    table.add(id2, val2);
    table.add(id3, val3);

    ASSERT_TRUE(table.get(id1) == val1);
    ASSERT_TRUE(table.get(id2) == val2);
    ASSERT_TRUE(table.get(id3) == val3);
}

TEST_F(rask_cg_SymbolTable, duplicatedSymbol)
{
    table.add(id1, val1);

    try
    {
        table.add(id1, val2);
        FAIL() << "expected SymbolTableError";
    }
    catch (const cg::SymbolTableError& e)
    {
        ASSERT_STREQ(e.what(), "Duplicated identifier");
    }

    ASSERT_TRUE(table.get(id1) == val1);
}

TEST_F(rask_cg_SymbolTable, badSymbol)
{
    try
    {
        table.get(id1);
        FAIL() << "expected SymbolTableError";
    }
    catch (const cg::SymbolTableError& e)
    {
        ASSERT_STREQ(e.what(), "Symbol \'x\' not found");
    }
}
