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
#include <rask/test/Mock.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <boost/scoped_ptr.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

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

struct genReturn_TestData
{
    llvm::LLVMContext ctx;
    rask::cg::SymbolTable st;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    CodeGeneratorMock cg;

    genReturn_TestData() : module(new llvm::Module("testModule", ctx)), cg(st)
    {
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }
};

typedef test_group<genReturn_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genReturn");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::Return ret(ast::Constant(10));

    llvm::AllocaInst *a1 = new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32));

    MOCK_RETURN(cg, genValue, a1);

    cg.genReturn(ret, *block, st, *module);

    ENSURE_CALL(cg, genValue(ret.expr(), st, *block));
    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(llvm::isa<llvm::ReturnInst>(block->front()));
    llvm::ReturnInst *inst = llvm::cast<llvm::ReturnInst>(&block->front());
    ENSURE(inst->getReturnValue() == a1);
}

}
