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
#include <rask/test/Mock.hpp>

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

MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
public:

    rask::cg::SymbolTable dummyST_;
    
    CodeGeneratorMock() : rask::cg::CodeGenerator(dummyST_) { }
    
    MOCK_METHOD(void, genFunction, (const rask::ast::CustomFunction&, f)(llvm::Module&, module));
    MOCK_METHOD(void, declFunction, (const rask::ast::CustomFunction&, f)(llvm::Module&, module));
    MOCK_METHOD(void, declBuiltinFunctions, (llvm::Module&, module));
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
    ast::SharedCustomFunction f1(new ast::CustomFunction(cst::Identifier::create(Position(), "abc")));
    ast::SharedCustomFunction f2(new ast::CustomFunction(cst::Identifier::create(Position(), "def")));
    ast::Tree ast;
    ast.add(f1);
    ast.add(f2);
    
    CodeGeneratorMock cg;
    
    std::auto_ptr<llvm::Module> module = cg.genModule(ast, context);

    ensure_equals("context", &module->getContext(), &context);

    ENSURE_CALL(cg, declBuiltinFunctions(*module));
    ENSURE_CALL(cg, declFunction(*f1, *module));
    ENSURE_CALL(cg, declFunction(*f2, *module));
    ENSURE_CALL(cg, genFunction(*f1, *module));
    ENSURE_CALL(cg, genFunction(*f2, *module));
}


}
