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
#include <rask/cg/CodeGenerator.hpp>
#include <rask/test/Mock.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace
{

MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
public:

    rask::cg::SymbolTable symbolTable;
    
    CodeGeneratorMock() : rask::cg::CodeGenerator(symbolTable) { }

    MOCK_METHOD(llvm::CallInst *, genFunctionCall, (const rask::ast::FunctionCall&, fc)(llvm::BasicBlock&, block)(llvm::Module&, module));
    MOCK_METHOD(llvm::AllocaInst *, genVariableDecl, (const rask::ast::VariableDecl&, vd)(llvm::BasicBlock&, block));
};
    
}

namespace tut
{

struct genFunction_TestData
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    CodeGeneratorMock cg;
    rask::ast::CustomFunction f;
    
    genFunction_TestData()
        : module(new llvm::Module("testModule", ctx)),
        f(rask::cst::Identifier::create(rask::Position(), "abc"))
    {
        cg.declBuiltinFunctions(*module);
        cg.declFunction(f, *module);
        MOCK_RETURN(cg, genFunctionCall, 0);
        MOCK_RETURN(cg, genVariableDecl, 0);
    }
};

typedef test_group<genFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);
    
    ENSURE_EQUALS(gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 1u);
    ENSURE(llvm::isa<llvm::ReturnInst>(entry->front()));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    f.addStmt(ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments()));
    f.addStmt(ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments()));
    
    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);

    ENSURE_EQUALS(gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 1u);
    ENSURE(llvm::isa<llvm::ReturnInst>(entry->front()));
    ENSURE_CALL(cg, genFunctionCall(getFunctionCall(f.stmt(0)), *entry, *module));
    ENSURE_CALL(cg, genFunctionCall(getFunctionCall(f.stmt(1)), *entry, *module));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    f.addStmt(ast::VariableDecl(cst::Identifier::create(Position(), "asia"), 1));
    f.addStmt(ast::VariableDecl(cst::Identifier::create(Position(), "kasia"), 2));
    
    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);
    
    ENSURE_EQUALS(gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 1u);
    ENSURE_CALL(cg, genVariableDecl(getVariableDecl(f.stmt(0)), *entry));
    ENSURE_CALL(cg, genVariableDecl(getVariableDecl(f.stmt(1)), *entry));
    ENSURE(llvm::isa<llvm::ReturnInst>(entry->front()));
}

}
