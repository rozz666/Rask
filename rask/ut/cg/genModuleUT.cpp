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
#include <rask/test/TUTAssert.hpp>
#include <rask/cg/CodeGenerator.hpp>

namespace tut
{

struct genModule_TestData
{
};

typedef test_group<genModule_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genModule");

class CodeGeneratorMock : public rask::cg::CodeGenerator
{
public:

    rask::ast::Function function;
    llvm::Function *genFunctionResult;
    int declBuiltinFunctionsCalled;
    int genFunctionCalled;
    int counter;

    CodeGeneratorMock() : genFunctionResult(0), declBuiltinFunctionsCalled(false), genFunctionCalled(0), counter(0) { }
    
    virtual llvm::Function *genFunction(const rask::ast::Function& f, llvm::Module *module)
    {
        genFunctionCalled = ++counter;
        function = f;
        
        genFunctionResult = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(module->getContext()), false), llvm::Function::ExternalLinkage, "stub", module);

        return genFunctionResult;
    }

    virtual void declBuiltinFunctions(llvm::Module *module)
    {
        declBuiltinFunctionsCalled = ++counter;
    }
};

}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    llvm::LLVMContext context;
    ast::Tree ast;

    CodeGeneratorMock cg;
    
    llvm::Module *module = cg.genModule(ast, context);

    ensure_equals("context", &module->getContext(), &context);
    ensure_equals("builtin", cg.declBuiltinFunctionsCalled, 1);
    ensure_equals("gen", cg.genFunctionCalled, 2);
    ensure_size("main", module->getFunctionList(), 1u);
    ensure_contains("stub", module->getFunctionList(), cg.genFunctionResult);
    ensure("function", cg.function == ast.main);
}


}
