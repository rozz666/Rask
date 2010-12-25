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
    typedef std::map<
        std::string,
        llvm::BinaryOperator *(*)(llvm::Value *left, llvm::Value *right, const llvm::Twine& name, llvm::BasicBlock *bb)
    > BinaryOpMap;
    static const BinaryOpMap binaryOpMap = boost::assign::map_list_of
        (BINARY_MINUS_NAME, BinaryOpMap::mapped_type(&llvm::BinaryOperator::CreateNSWSub))
        (BINARY_PLUS_NAME, BinaryOpMap::mapped_type(llvm::BinaryOperator::CreateNSWAdd))
        (BINARY_MULT_NAME, BinaryOpMap::mapped_type(llvm::BinaryOperator::CreateNSWMul));

    ast::SharedFunction f = fc.function().lock();

    if (f->name().value == UNARY_MINUS_NAME)
    {
        return llvm::BinaryOperator::CreateNeg(genValue(fc.args()[0], block), "", &block);
    }

    BinaryOpMap::const_iterator opGen = binaryOpMap.find(f->name().value);

    if (opGen != binaryOpMap.end())
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

    std::vector<llvm::Value *> args;

    BOOST_FOREACH(const ast::Expression& e, fc.args())
    {
        args.push_back(genValue(e, block));
    }

    return llvm::CallInst::Create(module.getFunction(f->name().value), args.begin(), args.end(), "", &block);
}
    
    
}
}