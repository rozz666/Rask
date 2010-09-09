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
#include <llvm/LLVMContext.h>
#include <llvm/DerivedTypes.h>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/test/TUTAssert.hpp>

namespace tut
{

struct declFunction_TestData
{
};

typedef test_group<declFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.declFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    llvm::LLVMContext context;
    std::auto_ptr<llvm::Module> module(new llvm::Module("testModule", context));
    rask::cg::SymbolTable st;
    rask::cg::CodeGenerator cg(st);
    ast::CustomFunction f(cst::Identifier::create(Position(), "f1"));

    cg.declFunction(f, *module);

    ensure_size("decl", module->getFunctionList(), 1u);
    llvm::Function& lf = module->getFunctionList().front();
    ensure_equals(lf.getNameStr(), f.name().value);
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    ensure("pointer type", llvm::isa<llvm::PointerType>(lf.getType()));
    ensure("type", lf.getType()->getElementType() == fType);
    ensure("no body", lf.getBasicBlockList().empty());
    ensure("C cc", lf.getCallingConv() == llvm::CallingConv::C);
    ensure_equals("linkage", lf.getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("module", lf.getParent(), module.get());
    ensure_equals("context", &lf.getContext(), &context);
}

}
