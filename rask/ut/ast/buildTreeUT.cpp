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
#include <rask/ast/Builder.hpp>

namespace
{
    
class BuilderMock : public rask::ast::Builder
{
public:

    rask::ast::SymbolTable st;
    const rask::cst::Function *functionDecl;
    const rask::cst::Function *function;
    int buildFunctionDeclCalled;
    int buildFunctionCalled;
    int counter;
    bool buildFunctionDeclSuccessful;
    bool buildFunctionSuccessful;
    rask::ast::SharedFunction buildFunctionDeclResult;

    BuilderMock(rask::error::Logger& logger)
        : rask::ast::Builder(logger, st), functionDecl(0), function(0), buildFunctionDeclCalled(0), buildFunctionCalled(0),
        counter(0), buildFunctionDeclSuccessful(true), buildFunctionSuccessful(true)
    {
    }

    virtual boost::optional<rask::ast::FunctionDecl> buildFunctionDecl(const rask::cst::Function& f)
    {
        buildFunctionDeclCalled = ++counter;
        functionDecl = &f;
        
        if (!buildFunctionDeclSuccessful) return boost::none;

        rask::ast::FunctionDecl fd(f.name);
        buildFunctionDeclResult = fd.function();

        return fd;
    }
    
    virtual bool buildFunction(const rask::cst::Function& f)
    {
        buildFunctionCalled = ++counter;
        function = &f;

        return buildFunctionSuccessful;
    }
};

}

namespace tut
{

struct buildAST_TestData
{
    rask::error::Logger logger;
    BuilderMock builder;
    rask::cst::Tree cst;

    buildAST_TestData() : builder(logger)
    {
        cst.functions.resize(1);
    }
};

typedef test_group<buildAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildTree");

}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
   
    boost::optional<ast::Tree> ast = builder.buildTree(cst);

    ensure("built", ast);
    ensure_equals("called 1", builder.buildFunctionDeclCalled, 1);
    ensure_equals("called 2", builder.buildFunctionCalled, 2);
    ensure("result", ast->main == builder.buildFunctionDeclResult);
    ensure("f 1", builder.functionDecl == &cst.functions[0]);
    ensure("f 2", builder.function == &cst.functions[0]);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    builder.buildFunctionSuccessful = false;

    ensure_not("not built", builder.buildTree(cst));
    ensure_equals("called", builder.buildFunctionDeclCalled, 1);
    ensure_equals("called", builder.buildFunctionCalled, 2);
    ensure("f 1", builder.functionDecl == &cst.functions[0]);
    ensure("f 2", builder.function == &cst.functions[0]);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    builder.buildFunctionDeclSuccessful = false;
    
    ensure_not("not built", builder.buildTree(cst));
    ensure_equals("called", builder.buildFunctionDeclCalled, 1);
    ensure_equals("not called", builder.buildFunctionCalled, 0);
    ensure("f 1", builder.functionDecl == &cst.functions[0]);
}

}
