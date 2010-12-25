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
#include <rask/Operators.hpp>

namespace rask
{
namespace cg
{
    
llvm::Value *CodeGenerator::genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block)
{
    ast::SharedFunction f = fc.function().lock();

    if (f->name().value == UNARY_MINUS_NAME)
    {
        return llvm::BinaryOperator::CreateNeg(genValue(fc.args()[0], block), "", &block);
    }

    if (f->name().value == BINARY_MINUS_NAME)
    {
        llvm::Value *left = genValue(fc.args()[0], block);
        llvm::Value *right = genValue(fc.args()[1], block);
        return llvm::BinaryOperator::CreateNSWSub(left, right, "", &block);
    }

    const llvm::Module& module = *block.getParent()->getParent();

    if (!module.getFunction(f->name().value))
    {
        throw std::invalid_argument("Function \'" + f->name().value + "\' not declared");
    }

    std::vector<llvm::Value *> args;

    BOOST_FOREACH(const ast::Expression& e, fc.args())
    {
        args.push_back(genValue(e, block));
    }

    return llvm::CallInst::Create(module.getFunction(f->name().value), args.begin(), args.end(), "", &block);
}
    
    
}
}