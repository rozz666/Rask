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
#include <rask/cg/CodeGenerator.hpp>

namespace rask
{
namespace cg
{

std::auto_ptr<llvm::Module> CodeGenerator::genModule(const ast::Tree& ast, llvm::LLVMContext& context)
{
    std::auto_ptr<llvm::Module> module(new llvm::Module("mainModule", context));

    declBuiltinFunctions(*module);
    
    for (std::size_t i = 0; i != ast.functionCount(); ++i)
    {
        declFunction(*ast.function(i), *module);
    }
    
    for (std::size_t i = 0; i != ast.functionCount(); ++i)
    {
        genFunction(*ast.function(i), *module);
    }
    
    return module;
}

}
}

