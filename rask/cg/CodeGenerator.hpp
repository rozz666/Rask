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

#include <rask/ast/Function.hpp>
#include <rask/ast/Tree.hpp>
#include <llvm/Function.h>
#include <llvm/Module.h>

namespace rask
{
namespace cg
{

class CodeGenerator
{
public:
        
    virtual llvm::Function *genFunctionIR(const ast::Function& f, llvm::Module *module);
    virtual void declBuiltinFunctions(llvm::Module *module);
    virtual llvm::Module *genModule(const ast::Tree& ast, llvm::LLVMContext& context);
};
    
}
}

#endif /* RASK_CG_CODEGENERATOR_HPP */