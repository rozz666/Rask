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

}
}

