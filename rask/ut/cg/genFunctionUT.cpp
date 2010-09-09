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
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>

namespace
{

class CodeGeneratorMock : public rask::cg::CodeGenerator
{
public:

    struct GenFunctionCall
    {
        int N;
        const rask::ast::FunctionCall *fc;
        llvm::BasicBlock *block;
        llvm::Module *module;
    };

    struct GenVariableDecl
    {
        int N;
        const rask::ast::VariableDecl *vd;
        llvm::BasicBlock *block;
    };
    
    int counter;
    std::vector<GenFunctionCall> genFunctionCallCalls;
    std::vector<GenVariableDecl> genVariableDeclCalls;
    rask::cg::SymbolTable symbolTable;
    
    CodeGeneratorMock() : rask::cg::CodeGenerator(symbolTable), counter(0) { }

    virtual llvm::CallInst *genFunctionCall(const rask::ast::FunctionCall& fc, llvm::BasicBlock& block, llvm::Module& module)
    {
        GenFunctionCall gfc = { ++counter, &fc, &block, &module };
        genFunctionCallCalls.push_back(gfc);
        return 0;
    }

    virtual llvm::AllocaInst *genVariableDecl(const rask::ast::VariableDecl& vd, llvm::BasicBlock& block)
    {
        GenVariableDecl gfd = { ++counter, &vd, &block };
        genVariableDeclCalls.push_back(gfd);
        return 0;
    }
};
    
}

namespace tut
{

struct genFunction_TestData
{
    llvm::LLVMContext ctx;
    std::auto_ptr<llvm::Module> module;
    CodeGeneratorMock cg;
    rask::ast::CustomFunction f;
    
    genFunction_TestData()
        : module(new llvm::Module("testModule", ctx)),
        f(rask::cst::Identifier::create(rask::Position(), "abc"))
    {
        cg.declBuiltinFunctions(*module);
        cg.declFunction(f, *module);
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
    
    ensure_equals("entry", gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ensure_equals("entry name", entry->getNameStr(), "entry");
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
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
    
    ensure_equals("entry", gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ensure_equals("entry name", entry->getNameStr(), "entry");
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure_equals("#genFC", cg.genFunctionCallCalls.size(), 2u);
    ensure_equals("genFC 1 N", cg.genFunctionCallCalls[0].N, 1);
    ensure_equals("genFC 1 fc", cg.genFunctionCallCalls[0].fc, &getFunctionCall(f.stmt(0)));
    ensure_equals("genFC 1 block", cg.genFunctionCallCalls[0].block, entry);
    ensure_equals("genFC 1 module", cg.genFunctionCallCalls[0].module, module.get());
    ensure_equals("genFC 2 N", cg.genFunctionCallCalls[1].N, 2);
    ensure_equals("genFC 2 fc", cg.genFunctionCallCalls[1].fc, &getFunctionCall(f.stmt(1)));
    ensure_equals("genFC 2 block", cg.genFunctionCallCalls[1].block, entry);
    ensure_equals("genFC 2 module", cg.genFunctionCallCalls[1].module, module.get());
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cst::Identifier name;
    name.value = "asia";
    ast::VariableDecl vd1(name, 1);
    name.value = "kasia";
    ast::VariableDecl vd2(name, 2);
    
    f.addStmt(vd1);
    f.addStmt(vd2);
    
    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);
    
    ensure_equals("entry", gf->getBasicBlockList().size(), 1u);
    llvm::BasicBlock *entry = &gf->getBasicBlockList().front();
    ensure_equals("entry name", entry->getNameStr(), "entry");
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure_equals("#genVD", cg.genVariableDeclCalls.size(), 2u);
    ensure_equals("genVD 1 N", cg.genVariableDeclCalls[0].N, 1);
    ensure_equals("genVD 1 vd", cg.genVariableDeclCalls[0].vd, &getVariableDecl(f.stmt(0)));
    ensure_equals("genVD 1 block", cg.genVariableDeclCalls[0].block, entry);
    ensure_equals("genVD 2 N", cg.genVariableDeclCalls[1].N, 2);
    ensure_equals("genVD 2 vd", cg.genVariableDeclCalls[1].vd, &getVariableDecl(f.stmt(1)));
    ensure_equals("genVD 2 block", cg.genVariableDeclCalls[1].block, entry);
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
}

}
