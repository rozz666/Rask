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
#include <boost/assign/list_of.hpp>

namespace rask
{
namespace cg
{

llvm::Value *CodeGenerator::genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block)
{
    ast::SharedFunction f = fc.function().lock();

    if (f->name().value == UNARY_MINUS_NAME)
    {
        return instructionFactory_->createNeg(genValue(fc.args()[0], block), &block);
    }

    BinaryOpMap::const_iterator opGen = binaryOpMap_.find(f->name().value);

    if (opGen != binaryOpMap_.end())
    {
        llvm::Value *left = genValue(fc.args()[0], block);
        llvm::Value *right = genValue(fc.args()[1], block);
        return opGen->second(left, right, "", &block);
    }

    const llvm::Module& module = *block.getParent()->getParent();

    if (!module.getFunction(f->name().value))
    {
        throw std::invalid_argument("Function \'" + f->name().value + "\' not declared");
    }

    InstructionFactory::Values args;

    BOOST_FOREACH(const ast::Expression& e, fc.args())
    {
        args.push_back(genValue(e, block));
    }

    return instructionFactory_->createCall(module.getFunction(f->name().value), args, &block);
}


}
}