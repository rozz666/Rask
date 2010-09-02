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

    const rask::cst::Function *function;
    int buildFunctionCalled;
    int counter;
    bool buildFunctionSuccessful;
    rask::ast::Function buildFunctionResult;

    BuilderMock(rask::error::Logger& logger)
        : rask::ast::Builder(logger), function(0), counter(0), buildFunctionSuccessful(true)
    {
        buildFunctionResult.addStmt(rask::ast::FunctionCall(1));
        buildFunctionResult.addStmt(rask::ast::FunctionCall(2));
        buildFunctionResult.addStmt(rask::ast::FunctionCall(3));
    }

    virtual boost::optional<rask::ast::Function> buildFunction(const rask::cst::Function& f)
    {
        buildFunctionCalled = ++counter;
        function = &f;

        if (!buildFunctionSuccessful) return boost::none;

        return buildFunctionResult;
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

    buildAST_TestData() : builder(logger) { }
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
    ensure("result", ast->main == builder.buildFunctionResult);
    ensure_equals("called", builder.buildFunctionCalled, 1);
    ensure("main", builder.function == &cst.main);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    builder.buildFunctionSuccessful = false;

    boost::optional<ast::Tree> ast = builder.buildTree(cst);
    
    ensure_not("not built", ast);
    ensure_equals("called", builder.buildFunctionCalled, 1);
    ensure("main", builder.function == &cst.main);
}

}
