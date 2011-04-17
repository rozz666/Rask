// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/SymbolTable.hpp>
#include <rask/ut/cg/InstructionFactoryMock.hpp>
#include <rask/ast/BuiltinFunction.hpp>
#include <rask/Operators.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <rask/null.hpp>
#include <gmock/gmock.h>
#include <boost/assign/list_of.hpp>

using namespace rask;
using namespace testing;

namespace
{

struct CodeGeneratorMock : cg::CodeGenerator
{
    CodeGeneratorMock(cg::SharedInstructionFactory instructionFactory)
        : cg::CodeGenerator(null, null, instructionFactory, null) { }

    MOCK_METHOD2(genValue, llvm::Value *(const ast::Expression&, llvm::BasicBlock&));
};

std::ostream& operator<<(std::ostream& os, const ast::Expression& expr)
{
    return os << "ast::Expression";
}

}

struct rask_cg_CodeGenerator_genFunctionCall : testing::Test
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    cg::SharedInstructionFactoryMock instructionFactory;
    CodeGeneratorMock cg;
    llvm::AllocaInst *value1;
    llvm::AllocaInst *value2;
    llvm::BinaryOperator *binaryOp;

    rask_cg_CodeGenerator_genFunctionCall()
        : module(new llvm::Module("testModule", ctx)),
        instructionFactory(new cg::InstructionFactoryMock),
        cg(instructionFactory),
        value1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32))),
        value2(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32))),
        binaryOp(llvm::BinaryOperator::Create(llvm::Instruction::Add, value1, value2))
    {
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }

    llvm::Function *createFunction(const std::string& name, unsigned numArgs)
    {
        std::vector<const llvm::Type*> args(numArgs, llvm::IntegerType::get(ctx, 32));
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), args, false);
        return llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, &*module);
    }

    void checkBinaryOperator(const std::string& opName, llvm::Instruction::BinaryOps op)
    {
        unsigned numArgs = 2;
        createFunction(opName, numArgs);
        ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(opName, ast::INT32, numArgs));
        ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

        EXPECT_CALL(cg, genValue(Ref(fc.args()[0]), Ref(*block)))
            .WillOnce(Return(value1));
        EXPECT_CALL(cg, genValue(Ref(fc.args()[1]), Ref(*block)))
            .WillOnce(Return(value2));
        EXPECT_CALL(*instructionFactory, createBinaryOperator(op, value1, value2, block))
            .WillOnce(Return(binaryOp));

        ASSERT_TRUE(cg.genFunctionCall(fc, *block) == binaryOp);
    }
};

TEST_F(rask_cg_CodeGenerator_genFunctionCall, functionWithTwoArgs)
{
    std::string functionName = "dummy";
    unsigned numArgs = 2;
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(functionName, ast::VOID, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));
    llvm::CallInst *callInst = llvm::CallInst::Create(createFunction(functionName, numArgs));

    EXPECT_CALL(cg, genValue(Ref(fc.args()[0]), Ref(*block)))
        .WillOnce(Return(value1));
    EXPECT_CALL(cg, genValue(Ref(fc.args()[1]), Ref(*block)))
        .WillOnce(Return(value2));
    cg::InstructionFactory::Values args = boost::assign::list_of(value1)(value2);
    EXPECT_CALL(*instructionFactory, createCall(module->getFunction(functionName), args, block))
        .WillOnce(Return(callInst));

    ASSERT_TRUE(cg.genFunctionCall(fc, *block) == callInst);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, functionWithNoArgs)
{
    std::string functionName = "dummy";
    unsigned numArgs = 0;
    createFunction(functionName, numArgs);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(functionName, ast::VOID, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments());

    cg::InstructionFactory::Values args;
    EXPECT_CALL(*instructionFactory, createCall(_, args, _))
        .Times(1);

    cg.genFunctionCall(fc, *block);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, functionNotDeclared)
{
    std::string functionName = "missing";
    unsigned numArgs = 2;
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(functionName, ast::VOID, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    try
    {
        cg.genFunctionCall(fc, *block);
        FAIL() << "expected invalid_argument";
    }
    catch (const std::invalid_argument& e)
    {
        ASSERT_EQ(std::string("Function \'") + functionName + "\' not declared", e.what());
    }
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, negOp)
{
    unsigned numArgs = 1;
    createFunction(UNARY_MINUS_NAME, numArgs);
    ast::SharedBuiltinFunction dummy(new ast::BuiltinFunction(UNARY_MINUS_NAME, ast::INT32, numArgs));
    ast::FunctionCall fc(dummy, ast::FunctionCall::Arguments(numArgs, ast::Constant(1)));

    EXPECT_CALL(cg, genValue(Ref(fc.args()[0]), Ref(*block)))
        .WillOnce(Return(value1));
    EXPECT_CALL(*instructionFactory, createNeg(value1, block))
        .WillOnce(Return(binaryOp));

    ASSERT_TRUE(cg.genFunctionCall(fc, *block) == binaryOp);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, subOp)
{
    checkBinaryOperator(BINARY_MINUS_NAME, llvm::Instruction::Sub);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, addOp)
{
    checkBinaryOperator(BINARY_PLUS_NAME, llvm::Instruction::Add);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, mulOp)
{
    checkBinaryOperator(BINARY_MULT_NAME, llvm::Instruction::Mul);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, divOp)
{
    checkBinaryOperator(BINARY_DIV_NAME, llvm::Instruction::SDiv);
}

TEST_F(rask_cg_CodeGenerator_genFunctionCall, modOp)
{
    checkBinaryOperator(BINARY_MOD_NAME, llvm::Instruction::SRem);
}

