// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_INSTRUCTIONFACTORY_HPP
#define RASK_CG_INSTRUCTIONFACTORY_HPP

#include <llvm/Instructions.h>
#include <boost/shared_ptr.hpp>

namespace rask
{
namespace cg
{

class InstructionFactory
{
public:

    virtual ~InstructionFactory() { }

    virtual llvm::ReturnInst *createReturn(llvm::LLVMContext &context, llvm::BasicBlock *insertAtEnd);
    virtual llvm::AllocaInst *createAlloca(const llvm::Type *type, const std::string& name, llvm::BasicBlock *insertAtEnd);
    virtual llvm::StoreInst *createStore(llvm::Value *val, llvm::Value *ptr, llvm::BasicBlock *insertAtEnd);
    virtual llvm::BranchInst *createBranch(llvm::BasicBlock *ifTrue, llvm::BasicBlock *insertAtEnd);
};

typedef boost::shared_ptr<InstructionFactory> SharedInstructionFactory;

}
}

#endif // RASK_CG_INSTRUCTIONFACTORY_HPP
