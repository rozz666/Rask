// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

void CodeGenerator::genReturn(const ast::Return& ret, llvm::BasicBlock& block)
{
    llvm::Value *val = genValue(ret.expr(), block);
    llvm::ReturnInst::Create(block.getContext(), val, &block);
}


}
}