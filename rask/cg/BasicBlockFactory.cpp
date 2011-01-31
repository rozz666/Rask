// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/BasicBlockFactory.hpp>

namespace rask
{
namespace cg
{

llvm::BasicBlock* BasicBlockFactory::createBasicBlock(
    llvm::LLVMContext& context, const std::string& name, llvm::Function* parent)
{
    return llvm::BasicBlock::Create(context, name, parent);
}
llvm::BasicBlock* BasicBlockFactory::createBasicBlockBefore(
    llvm::LLVMContext& context, const std::string& name, llvm::Function* parent, llvm::BasicBlock* before)
{
    return llvm::BasicBlock::Create(context, name, parent, before);
}

}
}
