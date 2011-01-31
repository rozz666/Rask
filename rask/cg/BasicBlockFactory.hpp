// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RASK_CG_BASICBLOCKFACTORY_HPP
#define RASK_CG_BASICBLOCKFACTORY_HPP
#include <llvm/BasicBlock.h>
#include <boost/shared_ptr.hpp>

namespace rask
{
namespace cg
{

class BasicBlockFactory
{
public:
    virtual llvm::BasicBlock *createBasicBlock(
        llvm::LLVMContext& context, const std::string& name, llvm::Function *parent);
    virtual llvm::BasicBlock *createBasicBlockBefore(
        llvm::LLVMContext& context, const std::string& name, llvm::Function *parent, llvm::BasicBlock *before);
};

typedef boost::shared_ptr<BasicBlockFactory> SharedBasicBlockFactory;

}
}
#endif // RASK_CG_BASICBLOCKFACTORY_HPP
