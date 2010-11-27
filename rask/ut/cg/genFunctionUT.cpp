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
#include <rask/test/FunctionFactory.hpp>
#include <rask/test/VariableDeclFactory.hpp>

namespace
{

MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
public:

    rask::cg::SymbolTable symbolTable;
    
    CodeGeneratorMock() : rask::cg::CodeGenerator(symbolTable) { }

    MOCK_METHOD(llvm::CallInst *, genFunctionCall,
        (const rask::ast::FunctionCall&, fc)(llvm::BasicBlock&, block)(llvm::Module&, module));
    MOCK_METHOD(llvm::AllocaInst *, genVariableDecl, (const rask::ast::VariableDecl&, vd)(llvm::BasicBlock&, block));
    MOCK_METHOD(void, genReturn,
        (const rask::ast::Return&, vd)(llvm::BasicBlock&, block)(const rask::cg::SymbolTable&, st)(llvm::Module&, module));
};
    
}

namespace tut
{

struct genFunction_TestData
{
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    CodeGeneratorMock cg;
    rask::test::FunctionFactory functionFactory;
    rask::test::VariableDeclFactory varDeclFactory;
    rask::ast::CustomFunction f;
    
    genFunction_TestData()
        : module(new llvm::Module("testModule", ctx)),
        f(functionFactory.create("abc"))
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
    
    ENSURE_EQUALS(gf->size(), 1u);
    llvm::BasicBlock *entry = &gf->front();
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

    ENSURE_EQUALS(gf->size(), 1u);
    llvm::BasicBlock *entry = &gf->front();
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

    f.addStmt(varDeclFactory.create("asia", 1));
    f.addStmt(varDeclFactory.create("kasia", 2));
    
    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);
    
    ENSURE_EQUALS(gf->size(), 1u);
    llvm::BasicBlock *entry = &gf->front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 1u);
    ENSURE_CALL(cg, genVariableDecl(getVariableDecl(f.stmt(0)), *entry));
    ENSURE_CALL(cg, genVariableDecl(getVariableDecl(f.stmt(1)), *entry));
    ENSURE(llvm::isa<llvm::ReturnInst>(entry->front()));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;

    ast::CustomFunction f = functionFactory.create("xxx");
    f.addArg(cst::Identifier::create(Position(), "asia"));
    f.addArg(cst::Identifier::create(Position(), "kasia"));
    cg.declFunction(f, *module);

    llvm::Function::arg_iterator it = module->getFunction(f.name().value)->arg_begin();
    llvm::Argument *arg1 = &*it;
    llvm::Argument *arg2 = &*++it;
    
    cg.genFunction(f, *module);
    
    llvm::Function *gf = module->getFunction(f.name().value);
    
    ENSURE_EQUALS(gf->size(), 2u);
    llvm::Function::iterator block = gf->begin();
    ENSURE_EQUALS(block->getNameStr(), "args");
    ENSURE_EQUALS(block->size(), 5u);
    llvm::BasicBlock::iterator inst = block->begin();

    ENSURE(llvm::isa<llvm::AllocaInst>(*inst));
    llvm::AllocaInst *alloc = llvm::cast<llvm::AllocaInst>(&*inst);
    ENSURE_EQUALS(alloc->getNameStr(), f.arg(0)->name().value);
    ENSURE(alloc->getAllocatedType() == llvm::IntegerType::get(ctx, 32));
    ENSURE(cg.symbolTable.get(f.arg(0)->name()) == alloc);
    ++inst;
    ENSURE(llvm::isa<llvm::StoreInst>(*inst));
    llvm::StoreInst *store = llvm::cast<llvm::StoreInst>(&*inst);
    ENSURE(store->getValueOperand() == arg1);
    ENSURE(store->getPointerOperand() == alloc);
    ++inst;
    ENSURE(llvm::isa<llvm::AllocaInst>(*inst));
    alloc = llvm::cast<llvm::AllocaInst>(&*inst);
    ENSURE_EQUALS(alloc->getNameStr(), f.arg(1)->name().value);
    ENSURE(alloc->getAllocatedType() == llvm::IntegerType::get(ctx, 32));
    ENSURE(cg.symbolTable.get(f.arg(1)->name()) == alloc);
    ++inst;
    ENSURE(llvm::isa<llvm::StoreInst>(*inst));
    store = llvm::cast<llvm::StoreInst>(&*inst);
    ENSURE(store->getValueOperand() == arg2);
    ENSURE(store->getPointerOperand() == alloc);
    ++inst;
    ENSURE(llvm::isa<llvm::BranchInst>(*inst));
    llvm::BranchInst *branch = llvm::cast<llvm::BranchInst>(&*inst);
    ENSURE(branch->isUnconditional());
    
    ++block;
    ENSURE(branch->getSuccessor(0) == &*block);
    ENSURE_EQUALS(block->getNameStr(), "entry");
    ENSURE_EQUALS(block->size(), 1u);
    ENSURE(llvm::isa<llvm::ReturnInst>(block->front()));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    f = functionFactory.create("asia", ast::INT32);
    cg.declFunction(f, *module);

    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);

    ENSURE_EQUALS(gf->size(), 1u);
    llvm::BasicBlock *entry = &gf->front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 0u);
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;

    f = functionFactory.create("asia", ast::INT32);
    cg.declFunction(f, *module);

    f.addStmt(ast::Return(ast::Constant(10)));
    f.addStmt(ast::Return(ast::Constant(20)));

    cg.genFunction(f, *module);
    llvm::Function *gf = module->getFunction(f.name().value);

    ENSURE_EQUALS(gf->size(), 1u);
    llvm::BasicBlock *entry = &gf->front();
    ENSURE_EQUALS(entry->getNameStr(), "entry");
    ENSURE_EQUALS(entry->size(), 0u);
    ENSURE_CALL(cg, genReturn(getReturn(f.stmt(0)), *entry, cg.symbolTable, *module));
    ENSURE_CALL(cg, genReturn(getReturn(f.stmt(1)), *entry, cg.symbolTable, *module));
}

}
