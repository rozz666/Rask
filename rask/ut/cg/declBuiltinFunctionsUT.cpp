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
#include <rask/test/TUTAssert.hpp>

namespace tut
{

struct declBuiltinFunctions_TestData
{
};

typedef test_group<declBuiltinFunctions_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.declBuiltinFunctions");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module(new llvm::Module("testModule", context));

    cg::SymbolTable st;
    cg::CodeGenerator(st).declBuiltinFunctions(*module);

    llvm::Function *printInt = module->getFunction("print");

    std::vector<const llvm::Type*> printIntArgs;
    printIntArgs.push_back(llvm::IntegerType::get(context, 32));
    llvm::FunctionType *printIntType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), printIntArgs, false);

    ENSURE(printInt);
    ensure("pointer type", llvm::isa<llvm::PointerType>(printInt->getType()));
    ensure("type", printInt->getType()->getElementType() == printIntType);
    ensure("no body", printInt->getBasicBlockList().empty());
    ensure("C cc", printInt->getCallingConv() == llvm::CallingConv::C);
    ensure_equals("linkage", printInt->getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("module", printInt->getParent(), module.get());
    ensure_equals("context", &printInt->getContext(), &context);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module(new llvm::Module("testModule", context));
    
    cg::SymbolTable st;
    cg::CodeGenerator(st).declBuiltinFunctions(*module);
    
    llvm::Function *getInt32 = module->getFunction("getInt32");
    
    std::vector<const llvm::Type*> printIntArgs;
    llvm::FunctionType *getInt32Type = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    
    ENSURE(getInt32);
    ensure("pointer type", llvm::isa<llvm::PointerType>(getInt32->getType()));
    ensure("type", getInt32->getType()->getElementType() == getInt32Type);
    ensure("no body", getInt32->getBasicBlockList().empty());
    ensure("C cc", getInt32->getCallingConv() == llvm::CallingConv::C);
    ensure_equals("linkage", getInt32->getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("module", getInt32->getParent(), module.get());
    ensure_equals("context", &getInt32->getContext(), &context);
}

}
