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

    struct GenVarDecl
    {
        int N;
        const rask::ast::VarDecl *vd;
        llvm::BasicBlock *block;
    };
    
    int counter;
    std::vector<GenFunctionCall> genFunctionCallCalls;
    std::vector<GenVarDecl> genVarDeclCalls;
    rask::cg::SymbolTable symbolTable;
    
    CodeGeneratorMock() : rask::cg::CodeGenerator(symbolTable), counter(0) { }

    virtual llvm::CallInst *genFunctionCall(const rask::ast::FunctionCall& fc, llvm::BasicBlock& block, llvm::Module& module)
    {
        GenFunctionCall gfc = { ++counter, &fc, &block, &module };
        genFunctionCallCalls.push_back(gfc);
        return 0;
    }

    virtual llvm::AllocaInst *genVarDecl(const rask::ast::VarDecl& vd, llvm::BasicBlock& block)
    {
        GenVarDecl gfd = { ++counter, &vd, &block };
        genVarDeclCalls.push_back(gfd);
        return 0;
    }
};
    
}

namespace tut
{

struct genFunctionIR_TestData
{
    rask::ast::Function f;
    llvm::LLVMContext ctx;
    std::auto_ptr<llvm::Module> module;
    CodeGeneratorMock cg;
    llvm::BasicBlock *entry;
    
    genFunctionIR_TestData() : module(new llvm::Module("testModule", ctx))
    {
        cg.declBuiltinFunctions(*module);
    }

    void ensureMainDef(llvm::Function *gf)
    {
        ensure("pointer type", llvm::isa<llvm::PointerType>(gf->getType()));
        ensure_equals("type", gf->getType()->getElementType(), llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false));
        ensure_equals("linkage", gf->getLinkage(), llvm::Function::ExternalLinkage);
        ensure_equals("name", gf->getNameStr(), "main");
        ensure_equals("module", gf->getParent(), module.get());
        ensure_equals("entry", gf->getBasicBlockList().size(), 1u);
        entry = &gf->getBasicBlockList().front();
        ensure_equals("entry name", entry->getNameStr(), "entry");
    }
};

typedef test_group<genFunctionIR_TestData> factory;
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

    llvm::Function *gf = cg.genFunction(f, *module);

    ensureMainDef(gf);
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
   
    f.addStmt(ast::FunctionCall(1));
    f.addStmt(ast::FunctionCall(2));
    
    llvm::Function *gf = cg.genFunction(f, *module);

    ensureMainDef(gf);
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure_equals("#genFC", cg.genFunctionCallCalls.size(), 2u);
    ensure_equals("genFC 1 N", cg.genFunctionCallCalls[0].N, 1);
    ensure_equals("genFC 1 fc", cg.genFunctionCallCalls[0].fc, &boost::get<ast::FunctionCall>(f.stmt(0)));
    ensure_equals("genFC 1 block", cg.genFunctionCallCalls[0].block, entry);
    ensure_equals("genFC 1 module", cg.genFunctionCallCalls[0].module, module.get());
    ensure_equals("genFC 2 N", cg.genFunctionCallCalls[1].N, 2);
    ensure_equals("genFC 2 fc", cg.genFunctionCallCalls[1].fc, &boost::get<ast::FunctionCall>(f.stmt(1)));
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
    ast::VarDecl vd1(name, 1);
    name.value = "kasia";
    ast::VarDecl vd2(name, 2);
    
    f.addStmt(vd1);
    f.addStmt(vd2);
    
    llvm::Function *gf = cg.genFunction(f, *module);
    
    ensureMainDef(gf);
    ensure_equals("1 instruction", entry->size(), 1u);
    ensure_equals("#genVD", cg.genVarDeclCalls.size(), 2u);
    ensure_equals("genVD 1 N", cg.genVarDeclCalls[0].N, 1);
    ensure_equals("genVD 1 vd", cg.genVarDeclCalls[0].vd, &boost::get<ast::VarDecl>(f.stmt(0)));
    ensure_equals("genVD 1 block", cg.genVarDeclCalls[0].block, entry);
    ensure_equals("genVD 2 N", cg.genVarDeclCalls[1].N, 2);
    ensure_equals("genVD 2 vd", cg.genVarDeclCalls[1].vd, &boost::get<ast::VarDecl>(f.stmt(1)));
    ensure_equals("genVD 2 block", cg.genVarDeclCalls[1].block, entry);
    ensure("ret", llvm::isa<llvm::ReturnInst>(entry->front()));
}

}
