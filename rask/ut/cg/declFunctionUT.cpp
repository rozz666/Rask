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
#include <boost/scoped_ptr.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/DerivedTypes.h>
#include <rask/cg/CodeGenerator.hpp>
#include <rask/cg/Prefixes.hpp>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/FunctionFactory.hpp>

namespace tut
{

struct declFunction_TestData
{
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module;
    rask::cg::SymbolTable st;
    rask::cg::CodeGenerator cg;
    rask::test::FunctionFactory functionFactory;

    declFunction_TestData() : module(new llvm::Module("testModule", context)), cg(st) { }
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

    ast::CustomFunction f = functionFactory.create("f1");

    cg.declFunction(f, *module);

    ENSURE_EQUALS(module->getFunctionList().size(), 1u);
    llvm::Function& lf = module->getFunctionList().front();
    ENSURE_EQUALS(lf.getNameStr(), f.name().value);
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), false);
    ENSURE(llvm::isa<llvm::PointerType>(lf.getType()));
    ENSURE(lf.getType()->getElementType() == fType);
    ENSURE(lf.getBasicBlockList().empty());
    ENSURE(lf.getCallingConv() == llvm::CallingConv::C);
    ENSURE_EQUALS(lf.getLinkage(), llvm::Function::ExternalLinkage);
    ENSURE(lf.getParent() == module.get());
    ENSURE(&lf.getContext() == &context);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::CustomFunction f = functionFactory.create("f1");
    f.addArg(cst::Identifier::create(Position(), "arg1"));
    f.addArg(cst::Identifier::create(Position(), "arg2"));

    cg.declFunction(f, *module);

    ENSURE_EQUALS(module->getFunctionList().size(), 1u);
    llvm::Function& lf = module->getFunctionList().front();
    ENSURE_EQUALS(lf.getNameStr(), f.name().value);
    std::vector<const llvm::Type *> fArgs(2, llvm::IntegerType::get(module->getContext(), 32));
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), fArgs, false);
    ENSURE(llvm::isa<llvm::PointerType>(lf.getType()));
    ENSURE(lf.getType()->getElementType() == fType);
    ENSURE_EQUALS(lf.arg_size(), 2u);
    llvm::Function::arg_iterator arg = lf.arg_begin();
    ENSURE_EQUALS(arg->getNameStr(), cg::ARG_PREFIX + f.arg(0)->name().value);
    ++arg;
    ENSURE_EQUALS(arg->getNameStr(), cg::ARG_PREFIX + f.arg(1)->name().value);
    ENSURE(lf.getBasicBlockList().empty());
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ast::CustomFunction f = functionFactory.create("f1", ast::INT32);

    cg.declFunction(f, *module);

    ENSURE_EQUALS(module->getFunctionList().size(), 1u);
    llvm::Function& lf = module->getFunctionList().front();
    ENSURE_EQUALS(lf.getNameStr(), f.name().value);
    llvm::FunctionType *fType = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    ENSURE(llvm::isa<llvm::PointerType>(lf.getType()));
    ENSURE(lf.getType()->getElementType() == fType);
    ENSURE(lf.getBasicBlockList().empty());
    ENSURE(lf.getCallingConv() == llvm::CallingConv::C);
    ENSURE_EQUALS(lf.getLinkage(), llvm::Function::ExternalLinkage);
    ENSURE(lf.getParent() == module.get());
    ENSURE(&lf.getContext() == &context);
}

}
