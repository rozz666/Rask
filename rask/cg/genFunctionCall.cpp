// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/foreach.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/DerivedTypes.h>
#include <llvm/Instructions.h>
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{
    
llvm::CallInst *CodeGenerator::genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block)
{
    ast::SharedFunction f = fc.function().lock();

    std::vector<llvm::Value *> args;
    
    BOOST_FOREACH(const ast::Expression& e, fc.args())
    {
        args.push_back(genValue(e, block));
    }

    const llvm::Module& module = *block.getParent()->getParent();
    
    return llvm::CallInst::Create(module.getFunction(f->name().value), args.begin(), args.end(), "", &block);
}
    
    
}
}