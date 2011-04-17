// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_BASICBLOCKFACTORYMOCK_HPP
#define RASK_CG_BASICBLOCKFACTORYMOCK_HPP

#include <rask/cg/BasicBlockFactory.hpp>
#include <gmock/gmock.h>

namespace rask
{
namespace cg
{

struct BasicBlockFactoryMock : cg::BasicBlockFactory
{
    MOCK_METHOD3(createBasicBlock, llvm::BasicBlock *(llvm::LLVMContext&, const std::string&, llvm::Function *));
    MOCK_METHOD4(
        createBasicBlockBefore,
        llvm::BasicBlock *(llvm::LLVMContext&, const std::string&, llvm::Function *, llvm::BasicBlock *));
};

typedef boost::shared_ptr<BasicBlockFactoryMock> SharedBasicBlockFactoryMock;

}
}
#endif /* RASK_CG_BASICBLOCKFACTORYMOCK_HPP */
