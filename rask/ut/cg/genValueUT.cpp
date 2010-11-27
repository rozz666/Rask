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
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>
#include <boost/scoped_ptr.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Instructions.h>
#include <llvm/DerivedTypes.h>
#include <rask/test/VariableFactory.hpp>

namespace
{

MOCK(CodeGeneratorMock, rask::cg::CodeGenerator)
{
public:

    CodeGeneratorMock(rask::cg::SymbolTable& symbolTable) : rask::cg::CodeGenerator(symbolTable) { }

    MOCK_METHOD(llvm::CallInst *, genFunctionCall,
        (const rask::ast::FunctionCall&, fc)(llvm::BasicBlock&, block)(llvm::Module&, module));
};

}

namespace tut
{

struct genValue_TestData
{
    rask::test::VariableFactory variableFactory;
    llvm::LLVMContext ctx;
    boost::scoped_ptr<llvm::Module> module;
    llvm::BasicBlock *block;
    rask::cg::SymbolTable st;
    CodeGeneratorMock cg;
    llvm::AllocaInst *a1;
    
    genValue_TestData()
        : module(new llvm::Module("testModule", ctx)), cg(st),
        a1(new llvm::AllocaInst(llvm::IntegerType::get(ctx, 32)))
    {
        llvm::FunctionType *type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), false);
        llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "main", &*module);
        block = llvm::BasicBlock::Create(ctx, "entry", func);
    }
};

typedef test_group<genValue_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.CodeGenerator.genValue");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    ast::Constant c(10);
    ENSURE(cg.genValue(c, st, *block) == llvm::ConstantInt::get(ctx, llvm::APInt(32, c, true)));
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    ast::SharedVariable v = variableFactory.createShared("x");
    st.add(v->name(), a1);
    
    llvm::Value *val = cg.genValue(v, st, *block);

    ENSURE(llvm::isa<llvm::LoadInst>(val));
    ENSURE(llvm::cast<llvm::LoadInst>(val)->getPointerOperand() == st.get(v->name()));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ast::Expression fc = ast::FunctionCall(ast::WeakFunction(), ast::FunctionCall::Arguments(0));

    MOCK_RETURN(cg, genFunctionCall, 0);

    llvm::Value *val = cg.genValue(fc, st, *block);

    ENSURE(val == 0);
    ENSURE_CALL(cg, genFunctionCall(getFunctionCall(fc), *block, *module));
}

}
