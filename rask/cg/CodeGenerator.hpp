// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_CODEGENERATOR_HPP
#define RASK_CG_CODEGENERATOR_HPP

#include <memory>
#include <rask/ast/CustomFunction.hpp>
#include <rask/ast/Tree.hpp>
#include <rask/cg/SymbolTable.hpp>
#include <rask/cg/BasicBlockFactory.hpp>
#include <rask/cg/InstructionFactory.hpp>
#include <llvm/Function.h>
#include <llvm/Module.h>
#include <llvm/Instructions.h>
#include <di/constructor.hpp>

namespace rask
{
namespace cg
{

class CodeGenerator
{
public:

    DI_CONSTRUCTOR(
        CodeGenerator, (
            SharedSymbolTable symbolTable,
            SharedBasicBlockFactory basicBlockFactory,
            SharedInstructionFactory instructionFactory));

    virtual llvm::Value *genFunctionCall(const ast::FunctionCall& fc, llvm::BasicBlock& block);
    virtual llvm::AllocaInst *genVariableDecl(const ast::VariableDecl& vd, llvm::BasicBlock& block);
    virtual void genFunction(const ast::CustomFunction& f, llvm::Module& module);
    virtual void declFunction(const ast::CustomFunction& f, llvm::Module& module);
    virtual void declBuiltinFunctions(llvm::Module& module);
    virtual void genModule(const ast::Tree& ast, llvm::LLVMContext& context, llvm::Module& module);
    virtual llvm::Value *genValue(const ast::Expression& expr, llvm::BasicBlock& block);
    virtual void genReturn(const ast::Return& ret, llvm::BasicBlock& block);

private:

    typedef std::map<
        std::string,
        llvm::BinaryOperator *(*)(llvm::Value *left, llvm::Value *right, const llvm::Twine& name, llvm::BasicBlock *bb)
    > BinaryOpMap;

    BinaryOpMap binaryOpMap_;
    SharedSymbolTable symbolTable_;
    SharedBasicBlockFactory basicBlockFactory_;
    SharedInstructionFactory instructionFactory_;
};

typedef boost::shared_ptr<CodeGenerator> SharedCodeGenerator;

}
}

#endif /* RASK_CG_CODEGENERATOR_HPP */