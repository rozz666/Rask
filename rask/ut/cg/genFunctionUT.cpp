// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/Prefixes.hpp>
#include <boost/scoped_ptr.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <rask/test/FunctionFactory.hpp>
#include <rask/test/VariableDeclFactory.hpp>
#include <rask/null.hpp>
#include <gmock/gmock.h>

using namespace rask;
using namespace testing;

namespace
{

struct BasicBlockFactoryMock : cg::BasicBlockFactory
{
    MOCK_METHOD3(createBasicBlock, llvm::BasicBlock *(llvm::LLVMContext&, const std::string&, llvm::Function *));
    MOCK_METHOD4(
        createBasicBlockBefore,
        llvm::BasicBlock *(llvm::LLVMContext&, const std::string&, llvm::Function *, llvm::BasicBlock *));
};

typedef boost::shared_ptr<BasicBlockFactoryMock> SharedBasicBlockFactoryMock;

struct InstructionFactoryMock : cg::InstructionFactory
{
    MOCK_METHOD2(createReturn, llvm::ReturnInst *(llvm::LLVMContext&, llvm::BasicBlock *));
    MOCK_METHOD3(createAlloca, llvm::AllocaInst *(const llvm::Type *, const std::string& , llvm::BasicBlock *));
    MOCK_METHOD3(createStore, llvm::StoreInst *(llvm::Value *, llvm::Value *, llvm::BasicBlock *));
    MOCK_METHOD2(createBranch, llvm::BranchInst *(llvm::BasicBlock *, llvm::BasicBlock *));
};

typedef boost::shared_ptr<InstructionFactoryMock> SharedInstructionFactoryMock;

struct CodeGeneratorMock : cg::CodeGenerator
{
    CodeGeneratorMock(
        cg::SymbolTable& symbolTable,
        cg::SharedBasicBlockFactory basicBlockFactory,
        cg::SharedInstructionFactory instructionFactory)
        : cg::CodeGenerator(symbolTable, basicBlockFactory, instructionFactory) { }

    MOCK_METHOD2(genFunctionCall, llvm::CallInst *(const ast::FunctionCall&, llvm::BasicBlock&));
    MOCK_METHOD2(genVariableDecl, llvm::AllocaInst *(const ast::VariableDecl&, llvm::BasicBlock&));
    MOCK_METHOD2(genReturn, void(const ast::Return&, llvm::BasicBlock&));
};

}

struct rask_cg_CodeGenerator_genFunction : testing::Test
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    cg::SymbolTable symbolTable;
    SharedBasicBlockFactoryMock basicBlockFactory;
    SharedInstructionFactoryMock instructionFactory;
    CodeGeneratorMock cg;
    test::FunctionFactory functionFactory;
    test::VariableDeclFactory varDeclFactory;
    ast::CustomFunction f;
    llvm::BasicBlock *entry;


    rask_cg_CodeGenerator_genFunction()
        : module(new llvm::Module("testModule", ctx)), basicBlockFactory(new BasicBlockFactoryMock),
        instructionFactory(new InstructionFactoryMock), cg(symbolTable, basicBlockFactory, instructionFactory),
        f(functionFactory.create("abc")), entry(llvm::BasicBlock::Create(ctx))
    {
        cg.declBuiltinFunctions(*module);
        cg.declFunction(f, *module);
    }
};

TEST_F(rask_cg_CodeGenerator_genFunction, emptyFunction)
{
    llvm::Function *gf = module->getFunction(f.name().value);

    EXPECT_CALL(*basicBlockFactory, createBasicBlock(Ref(ctx), Eq<std::string>("entry"), gf))
        .WillOnce(Return(entry));
    EXPECT_CALL(*instructionFactory, createReturn(Ref(ctx), entry))
        .WillOnce(Return(null));

    cg.genFunction(f, *module);
}

TEST_F(rask_cg_CodeGenerator_genFunction, functionCalls)
{
    f.addStmt(ast::FunctionCall(null, ast::FunctionCall::Arguments()));
    f.addStmt(ast::FunctionCall(null, ast::FunctionCall::Arguments()));

    EXPECT_CALL(*basicBlockFactory, createBasicBlock(_, _, _))
        .WillOnce(Return(entry));
    {
        InSequence seq;
        EXPECT_CALL(cg, genFunctionCall(Ref(getFunctionCall(f.stmt(0))), Ref(*entry)))
            .WillOnce(Return(null));
        EXPECT_CALL(cg, genFunctionCall(Ref(getFunctionCall(f.stmt(1))), Ref(*entry)))
            .WillOnce(Return(null));
        EXPECT_CALL(*instructionFactory, createReturn(_, _))
            .WillOnce(Return(null));
    }

    cg.genFunction(f, *module);

}

TEST_F(rask_cg_CodeGenerator_genFunction, variableDeclarations)
{
    f.addStmt(varDeclFactory.create("asia"));
    f.addStmt(varDeclFactory.create("kasia"));
    EXPECT_CALL(*basicBlockFactory, createBasicBlock(_, _, _))
        .WillOnce(Return(entry));
    {
        InSequence seq;
        EXPECT_CALL(cg, genVariableDecl(Ref(getVariableDecl(f.stmt(0))), Ref(*entry)))
                .WillOnce(Return(null));
        EXPECT_CALL(cg, genVariableDecl(Ref(getVariableDecl(f.stmt(1))), Ref(*entry)))
                .WillOnce(Return(null));
        EXPECT_CALL(*instructionFactory, createReturn(_, _))
            .WillOnce(Return(null));
    }

    cg.genFunction(f, *module);
}

TEST_F(rask_cg_CodeGenerator_genFunction, functionWithArgs)
{
    ast::SharedCustomFunction f = functionFactory.createShared("xxx", ast::VOID, 2);
    cg.declFunction(*f, *module);

    llvm::Function::arg_iterator it = module->getFunction(f->name().value)->arg_begin();
    llvm::Argument *arg1 = &*it;
    llvm::Argument *arg2 = &*++it;

    EXPECT_CALL(*basicBlockFactory, createBasicBlock(_, _, _))
        .WillOnce(Return(entry));

    llvm::BasicBlock *argsBlock = llvm::BasicBlock::Create(ctx);

    EXPECT_CALL(*basicBlockFactory, createBasicBlockBefore(Ref(ctx), Eq<std::string>("args"), _, entry))
        .WillOnce(Return(argsBlock));

    const llvm::Type *type = llvm::IntegerType::get(ctx, 32);

    llvm::AllocaInst *alloca1 = new llvm::AllocaInst(type);
    llvm::AllocaInst *alloca2 = new llvm::AllocaInst(type);

    ExpectationSet init;

    init += EXPECT_CALL(*instructionFactory, createAlloca(type, cg::LOCAL_ARG_PREFIX + f->arg(0)->name().value, argsBlock))
        .WillOnce(Return(alloca1));
    init += EXPECT_CALL(*instructionFactory, createAlloca(type, cg::LOCAL_ARG_PREFIX + f->arg(1)->name().value, argsBlock))
        .WillOnce(Return(alloca2));

    init += EXPECT_CALL(*instructionFactory, createStore(arg1, alloca1, argsBlock))
        .WillOnce(Return(null));
    init += EXPECT_CALL(*instructionFactory, createStore(arg2, alloca2, argsBlock))
        .WillOnce(Return(null));

    EXPECT_CALL(*instructionFactory, createBranch(entry, argsBlock))
        .After(init)
        .WillOnce(Return(null));

    EXPECT_CALL(*instructionFactory, createReturn(_, _))
        .WillOnce(Return(null));

    cg.genFunction(*f, *module);
}

TEST_F(rask_cg_CodeGenerator_genFunction, int32Function)
{
    f = functionFactory.create("asia", ast::INT32);
    cg.declFunction(f, *module);

    EXPECT_CALL(*basicBlockFactory, createBasicBlock(_, _, _))
        .WillOnce(Return(entry));

    cg.genFunction(f, *module);
}

TEST_F(rask_cg_CodeGenerator_genFunction, returnStatements)
{
    f = functionFactory.create("asia", ast::INT32);
    cg.declFunction(f, *module);

    f.addStmt(ast::Return(ast::Constant(10)));
    f.addStmt(ast::Return(ast::Constant(20)));

    EXPECT_CALL(*basicBlockFactory, createBasicBlock(_, _, _))
        .WillOnce(Return(entry));
    {
        InSequence seq;
        EXPECT_CALL(cg, genReturn(Ref(getReturn(f.stmt(0))), Ref(*entry)));
        EXPECT_CALL(cg, genReturn(Ref(getReturn(f.stmt(1))), Ref(*entry)));
    }

    cg.genFunction(f, *module);
}
