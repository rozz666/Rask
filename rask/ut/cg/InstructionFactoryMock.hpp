// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_INSTRUCTIONFACTORYMOCK_HPP
#define RASK_CG_INSTRUCTIONFACTORYMOCK_HPP

#include <rask/cg/InstructionFactory.hpp>
#include <gmock/gmock.h>

namespace rask
{
namespace cg
{

class InstructionFactoryMock : public InstructionFactory {
public:
    MOCK_METHOD2(createReturn, llvm::ReturnInst*(llvm::LLVMContext &context, llvm::BasicBlock *insertAtEnd));
    MOCK_METHOD3(
        createAlloca, llvm::AllocaInst*(const llvm::Type *type, const std::string& name, llvm::BasicBlock *insertAtEnd));
    MOCK_METHOD3(createStore, llvm::StoreInst*(llvm::Value *val, llvm::Value *ptr, llvm::BasicBlock *insertAtEnd));
    MOCK_METHOD2(createBranch, llvm::BranchInst*(llvm::BasicBlock *ifTrue, llvm::BasicBlock *insertAtEnd));
    MOCK_METHOD2(createNeg, llvm::BinaryOperator*(llvm::Value *op, llvm::BasicBlock *insertAtEnd));
    MOCK_METHOD3(createCall,llvm::CallInst*(llvm::Value *func, const Values& args, llvm::BasicBlock* insertAtEnd));
    MOCK_METHOD4(
        createBinaryOperator,
        llvm::BinaryOperator*(
            llvm::Instruction::BinaryOps op, llvm::Value *s1, llvm::Value *s2, llvm::BasicBlock *insertAtEnd));
};

typedef boost::shared_ptr<InstructionFactoryMock> SharedInstructionFactoryMock;

}
}
#endif /* RASK_CG_INSTRUCTIONFACTORYMOCK_HPP */
