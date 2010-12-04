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
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/Prefixes.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/BasicBlock.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>

namespace
{
    
MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
public:

    CodeGeneratorMock(rask::cg::SymbolTable& st) : rask::cg::CodeGenerator(st) { }

    MOCK_METHOD(
        llvm::Value *, genValue,
        (const rask::ast::Expression&, expr)(const rask::cg::SymbolTable&, symbolTable)
        (llvm::BasicBlock&, block));
};
    
}

namespace tut
{

struct genVariableDecl_TestData
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::BasicBlock> block;
    rask::cg::SymbolTable st;
    CodeGeneratorMock cg;
    
    genVariableDecl_TestData()
        : block(llvm::BasicBlock::Create(ctx)), cg(st)
    {
    }
};

typedef test_group<genVariableDecl_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genVariableDecl");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cst::Identifier name;
    name.value = "asia";
    ast::VariableDecl vd(name, 10);
    
    llvm::Value *value = llvm::ConstantInt::get(ctx, llvm::APInt(32, getConstant(vd.value()), true));
    MOCK_RETURN(cg, genValue, value);
    
    llvm::AllocaInst *alloc = cg.genVariableDecl(vd, *block);

    ENSURE_EQUALS(block->size(), 2u);
    llvm::BasicBlock::iterator it = block->begin();

    ENSURE(&*it == alloc);
    ENSURE_EQUALS(alloc->getNameStr(), cg::LOCAL_VAR_PREFIX + name.value);
    ENSURE(alloc->getAllocatedType() == llvm::IntegerType::get(ctx, 32));
    ENSURE(st.get(name) == alloc);
    ++it;

    ENSURE_CALL(cg, genValue(vd.value(), st, *block));
    ENSURE(llvm::isa<llvm::StoreInst>(*it));
    llvm::StoreInst *store = llvm::cast<llvm::StoreInst>(&*it);
    ENSURE(store->getValueOperand() == value);
    ENSURE(store->getPointerOperand() == alloc);
}

}
