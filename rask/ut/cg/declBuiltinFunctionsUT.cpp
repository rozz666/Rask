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
    llvm::LLVMContext context;
    boost::scoped_ptr<llvm::Module> module;
    rask::cg::SymbolTable st;

    declBuiltinFunctions_TestData() : module(new llvm::Module("testModule", context))
    {
        rask::cg::CodeGenerator(st).declBuiltinFunctions(*module);
    }
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
    
    llvm::Function *printInt = module->getFunction("print");

    std::vector<const llvm::Type*> printIntArgs;
    printIntArgs.push_back(llvm::IntegerType::get(context, 32));
    llvm::FunctionType *printIntType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), printIntArgs, false);

    ENSURE(printInt);
    ENSURE(llvm::isa<llvm::PointerType>(printInt->getType()));
    ENSURE(printInt->getType()->getElementType() == printIntType);
    ENSURE(printInt->getBasicBlockList().empty());
    ENSURE(printInt->getLinkage() == llvm::Function::ExternalLinkage);
    ENSURE(printInt->getCallingConv() == llvm::CallingConv::C);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    llvm::Function *getInt32 = module->getFunction("getInt32");
   
    llvm::FunctionType *getInt32Type = llvm::FunctionType::get(llvm::IntegerType::get(context, 32), false);
    
    ENSURE(getInt32);
    ENSURE(llvm::isa<llvm::PointerType>(getInt32->getType()));
    ENSURE(getInt32->getType()->getElementType() == getInt32Type);
    ENSURE(getInt32->getBasicBlockList().empty());
    ENSURE(getInt32->getLinkage() == llvm::Function::ExternalLinkage);
    ENSURE(getInt32->getCallingConv() == llvm::CallingConv::C);
}

}
