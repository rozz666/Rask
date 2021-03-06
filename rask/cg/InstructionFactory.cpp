// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/InstructionFactory.hpp>

namespace rask
{
namespace cg
{

llvm::ReturnInst* InstructionFactory::createReturn(llvm::LLVMContext& context, llvm::BasicBlock* insertAtEnd)
{
    return llvm::ReturnInst::Create(context, insertAtEnd);
}

llvm::AllocaInst* InstructionFactory::createAlloca(
    const llvm::Type* type, const std::string& name, llvm::BasicBlock* insertAtEnd)
{
    return new llvm::AllocaInst(type, name, insertAtEnd);
}

llvm::StoreInst* InstructionFactory::createStore(llvm::Value* val, llvm::Value* ptr, llvm::BasicBlock* insertAtEnd)
{
    return new llvm::StoreInst(val, ptr, insertAtEnd);
}

llvm::BranchInst* InstructionFactory::createBranch(llvm::BasicBlock* ifTrue, llvm::BasicBlock* insertAtEnd)
{
    return llvm::BranchInst::Create(ifTrue, insertAtEnd);
}

llvm::BinaryOperator* InstructionFactory::createNeg(llvm::Value* op, llvm::BasicBlock* insertAtEnd)
{
    return llvm::BinaryOperator::CreateNeg(op, "", insertAtEnd);
}

llvm::CallInst* InstructionFactory::createCall(
    llvm::Value* func, const InstructionFactory::Values& args, llvm::BasicBlock* insertAtEnd)
{
    return llvm::CallInst::Create(func, args.begin(), args.end(), "", insertAtEnd);
}

llvm::BinaryOperator* InstructionFactory::createBinaryOperator(
    llvm::Instruction::BinaryOps op, llvm::Value* s1, llvm::Value* s2, llvm::BasicBlock* insertAtEnd)
{
    return llvm::BinaryOperator::Create(op, s1, s2, "", insertAtEnd);
}

}
}

