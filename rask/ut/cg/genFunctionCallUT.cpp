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
#include <rask/Operators.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace
{

MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
    CodeGeneratorMock(rask::cg::SymbolTable& st) : rask::cg::CodeGenerator(st) { }

    MOCK_METHOD(llvm::Value *, genValue, (const rask::ast::Expression&, expr)(llvm::BasicBlock&, block))
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

    void checkBinaryOperator(const std::string& opName, llvm::Instruction::BinaryOps op)
    {
        using namespace rask;

        unsigned numArgs = 2;
        createFunction(opName, numArgs);
        ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(opName, ast::INT32, numArgs));
        ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

        MOCK_RETURN(cg, genValue, a1);
        MOCK_RETURN(cg, genValue, a2);

        llvm::Value *val = cg.genFunctionCall(fc, *block);

        ENSURE_EQUALS(block->size(), 1u);
        ENSURE(val == &*block->begin());
        ENSURE(llvm::isa<llvm::BinaryOperator>(val));
        llvm::BinaryOperator *binOp = llvm::cast<llvm::BinaryOperator>(val);
        ENSURE(binOp->getOpcode() == op);
        ENSURE_EQUALS(binOp->getNumOperands(), 2u);
        ENSURE(binOp->getOperand(0) == a1);
        ENSURE(binOp->getOperand(1) == a2);
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
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(f, ast::VOID, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    MOCK_RETURN(cg, genValue, a1);
    MOCK_RETURN(cg, genValue, a2);

    llvm::Value *value = cg.genFunctionCall(fc, *block);

    ENSURE(llvm::isa<llvm::CallInst>(value));
    llvm::CallInst *call = llvm::cast<llvm::CallInst>(value);

    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(call == &*block->begin());
    ENSURE(call->getCalledFunction() == module->getFunction(f));
    ENSURE_EQUALS(call->getNumOperands(), numArgs + 1);
    ENSURE(call->getArgOperand(0) == a1);
    ENSURE(call->getArgOperand(1) == a2);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
    ENSURE_CALL(cg, genValue(fc.args()[0], *block));
    ENSURE_CALL(cg, genValue(fc.args()[1], *block));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    std::string f = "dummy";
    createFunction(f, 0);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(f, ast::VOID, 0));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());

    llvm::Value *value = cg.genFunctionCall(fc, *block);

    ENSURE(llvm::isa<llvm::CallInst>(value));
    llvm::CallInst *call = llvm::cast<llvm::CallInst>(value);
    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(call == &*block->begin());
    ENSURE(call->getCalledFunction() == module->getFunction(f));
    ENSURE_EQUALS(call->getNumOperands(), 1u);
    ENSURE(call->getCallingConv() == llvm::CallingConv::C);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    std::string f = "missing";
    unsigned numArgs = 2;
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(f, ast::VOID, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    try
    {
        cg.genFunctionCall(fc, *block);
        FAIL("expected invalid_argument");
    }
    catch (const std::invalid_argument& e)
    {
        ENSURE_EQUALS(e.what(), std::string("Function \'") + f + "\' not declared");
    }

    ENSURE_NO_CALLS(cg, genValue);
    ENSURE_EQUALS(block->size(), 0u);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    unsigned numArgs = 1;
    createFunction(UNARY_MINUS_NAME, numArgs);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(UNARY_MINUS_NAME, ast::INT32, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    MOCK_RETURN(cg, genValue, a1);

    llvm::Value *op = cg.genFunctionCall(fc, *block);

    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(op == &*block->begin());
    ENSURE(llvm::BinaryOperator::isNeg(op));
    ENSURE(llvm::BinaryOperator::getNegArgument(op) == a1);
    ENSURE_CALL(cg, genValue(fc.args()[0], *block));
}

template <>
template <>
void object::test<5>()
{
    checkBinaryOperator(rask::BINARY_MINUS_NAME, llvm::Instruction::Sub);
}

template <>
template <>
void object::test<6>()
{
    checkBinaryOperator(rask::BINARY_PLUS_NAME, llvm::Instruction::Add);
}

template <>
template <>
void object::test<7>()
{
    checkBinaryOperator(rask::BINARY_MULT_NAME, llvm::Instruction::Mul);
}

template <>
template <>
void object::test<8>()
{
    checkBinaryOperator(rask::BINARY_DIV_NAME, llvm::Instruction::SDiv);
}

template <>
template <>
void object::test<9>()
{
    checkBinaryOperator(rask::BINARY_MOD_NAME, llvm::Instruction::SRem);
}

}
