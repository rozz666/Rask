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
    
    virtual llvm::Function *genFunctionIR(const rask::ast::Function& f, llvm::Module *module)
    {
        function = f;
        
        return llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(module->getContext()), false), llvm::Function::ExternalLinkage, "stub", module);
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
    ensure_equals("main", module->getFunctionList().size(), 1u);
    ensure_equals("stub", module->getFunctionList().front().getNameStr(), "stub");
    ensure("function", cg.function == ast.main);
}


}
