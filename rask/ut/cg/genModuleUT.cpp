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

    struct GenFunction
    {
        int N;
        const rask::ast::CustomFunction *f;
        llvm::Module *module;
    };
    
    struct DeclFunction
    {
        int N;
        const rask::ast::CustomFunction *f;
        llvm::Module *module;
    };
    
    int counter;
    int declBuiltinFunctionsCalled;
    llvm::Module *declBuiltinFunctionsModule;
    std::vector<DeclFunction> declFunctionCalls;
    std::vector<GenFunction> genFunctionCalls;
    
    CodeGeneratorMock()
        : rask::cg::CodeGenerator(dummyST_), counter(0), 
        declBuiltinFunctionsCalled(0), declBuiltinFunctionsModule(0) { }
    
    virtual void genFunction(const rask::ast::CustomFunction& f, llvm::Module& module)
    {
        GenFunction gf = { ++counter, &f, &module };

        genFunctionCalls.push_back(gf);
    }

    virtual void declFunction(const rask::ast::CustomFunction& f, llvm::Module& module)
    {
        DeclFunction df = { ++counter, &f, &module };
        
        declFunctionCalls.push_back(df);
    }

    virtual void declBuiltinFunctions(llvm::Module& module)
    {
        declBuiltinFunctionsCalled = ++counter;
        declBuiltinFunctionsModule = &module;
    }
    
private:
        
    rask::cg::SymbolTable dummyST_;
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
    ast::SharedCustomFunction f1(new ast::CustomFunction(cst::Identifier::create(Position(), "abc")));
    ast::SharedCustomFunction f2(new ast::CustomFunction(cst::Identifier::create(Position(), "def")));
    ast.add(f1);
    ast.add(f2);
    
    CodeGeneratorMock cg;
    
    std::auto_ptr<llvm::Module> module = cg.genModule(ast, context);

    ensure_equals("context", &module->getContext(), &context);

    ensure_equals("builtin", cg.declBuiltinFunctionsCalled, 1);

    ensure_equals("2 decls", cg.declFunctionCalls.size(), 2u);
    ensure("decl 1 N", cg.declFunctionCalls[0].N == 2);
    ensure("decl 1 f", cg.declFunctionCalls[0].f == f1.get());
    ensure("decl 1 m", cg.declFunctionCalls[0].module == module.get());
    ensure("decl 2 N", cg.declFunctionCalls[1].N == 3);
    ensure("decl 2 f", cg.declFunctionCalls[1].f == f2.get());
    ensure("decl 2 m", cg.declFunctionCalls[1].module == module.get());
    
    ensure_equals("2 gens", cg.genFunctionCalls.size(), 2u);
    ensure("gen 1 N", cg.genFunctionCalls[0].N == 4);
    ensure("gen 1 f", cg.genFunctionCalls[0].f == f1.get());
    ensure("gen 1 m", cg.genFunctionCalls[0].module == module.get());
    ensure("gen 2 N", cg.genFunctionCalls[1].N == 5);
    ensure("gen 2 f", cg.genFunctionCalls[1].f == f2.get());
    ensure("gen 2 m", cg.genFunctionCalls[1].module == module.get());
}


}
