// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/DerivedTypes.h>

namespace tut
{

struct genFunctionIR_TestData
{
};

typedef test_group<genFunctionIR_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genFunctionIR");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::Function f;
    
    llvm::LLVMContext ctx;
    llvm::Module *module = new llvm::Module("testModule", ctx);

    llvm::Function *gf = cg::CodeGenerator().genFunctionIR(f, module);

    ensure("pointer type", llvm::isa<llvm::PointerType>(gf->getType()));
    ensure_equals("type", gf->getType()->getElementType(), llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false));
    ensure_equals("linkage", gf->getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("name", gf->getNameStr(), "main");
    ensure_equals("module", gf->getParent(), module);
    ensure_equals("no code", gf->getBasicBlockList().size(), 0u);
    ensure_equals("context", &gf->getContext(), &ctx);
}

}
