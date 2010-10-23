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
#include <rask/cg/SymbolTable.hpp>
#include <rask/ast/BuiltinFunction.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>
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
        (llvm::BasicBlock&, block)(llvm::Module&, module));
};
    
}

namespace tut
{

struct genFunctionCall_TestData
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    rask::cg::SymbolTable st;
    CodeGeneratorMock cg;
    llvm::AllocaInst *a1;
    llvm::AllocaInst *a2;
    
    genFunctionCall_TestData()
        : module(new llvm::Module("testModule", ctx)), cg(st),
        a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32))),
        a2(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32)))
    {
        cg.declBuiltinFunctions(*module);
        
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }

    void createFunction(const std::string& name, unsigned numArgs)
    {
        std::vector<const llvm::Type*> args(numArgs, llvm::IntegerType::get(ctx, 32));
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), args, false);
        llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, &*module);
    }
};

typedef test_group<genFunctionCall_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genFunctionCall");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    std::string f = "dummy";
    unsigned numArgs = 2;
    createFunction(f, numArgs);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(f, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    MOCK_RETURN(cg, genValue, a1);
    MOCK_RETURN(cg, genValue, a2);
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);

    ENSURE_CALL(cg, genValue(fc.args()[0], st, *block, *module));
    ENSURE_CALL(cg, genValue(fc.args()[1], st, *block, *module));
    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(call == &*block->begin());
    ENSURE(call->getCalledFunction() == module->getFunction(f));
    ENSURE_EQUALS(call->getNumOperands(), numArgs + 1);
    ENSURE(call->getArgOperand(0) == a1);
    ENSURE(call->getArgOperand(1) == a2);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    std::string f = "dummy";
    createFunction(f, 0);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(f, 0));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());
    
    llvm::CallInst *call = cg.genFunctionCall(fc, *block, *module);
    
    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(call == &*block->begin());
    ENSURE(call->getCalledFunction() == module->getFunction(f));
    ENSURE_EQUALS(call->getNumOperands(), 1u);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

}
