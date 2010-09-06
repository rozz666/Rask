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
    std::auto_ptr<llvm::Module> module(new llvm::Module("testModule", context));

    cg::SymbolTable st;
    cg::CodeGenerator(st).declBuiltinFunctions(*module);

    ensure_size("decl", module->getFunctionList(), 1u);

    llvm::Function& printInt = module->getFunctionList().front();

    std::vector<const llvm::Type*> printIntArgs;
    printIntArgs.push_back(llvm::IntegerType::get(context, 32));
    llvm::FunctionType *printIntType = llvm::FunctionType::get(llvm::Type::getVoidTy(context), printIntArgs, false);

    ensure_equals(printInt.getNameStr(), "_rask_print_int");
    ensure("pointer type", llvm::isa<llvm::PointerType>(printInt.getType()));
    ensure("type", printInt.getType()->getElementType() == printIntType);
    ensure("no body", printInt.getBasicBlockList().empty());
    ensure("C cc", printInt.getCallingConv() == llvm::CallingConv::C);
    ensure_equals("linkage", printInt.getLinkage(), llvm::Function::ExternalLinkage);
    ensure_equals("module", printInt.getParent(), module.get());
    ensure_equals("context", &printInt.getContext(), &context);
}

}
