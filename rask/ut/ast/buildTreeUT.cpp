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

namespace tut
{

struct buildAST_TestData
{
};

typedef test_group<buildAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildTree");

class BuilderMock : public rask::ast::Builder
{
public:

    const rask::cst::Function *function;
    rask::error::Logger *logger;
    int buildFunctionCalled;
    int counter;
    bool buildFunctionSuccessful;
    rask::ast::Function buildFunctionResult;

    BuilderMock() : function(0), logger(0), counter(0), buildFunctionSuccessful(true)
    {
        buildFunctionResult.addValue(1);
        buildFunctionResult.addValue(2);
        buildFunctionResult.addValue(3);
    }
    
    virtual boost::optional<rask::ast::Function> buildFunction(const rask::cst::Function& f, rask::error::Logger& el)
    {
        buildFunctionCalled = ++counter;
        function = &f;
        logger = &el;

        if (!buildFunctionSuccessful) return boost::none;

        return buildFunctionResult;
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
    
    BuilderMock builder;

    error::Logger logger;
    cst::Tree cst;
   
    boost::optional<ast::Tree> ast = builder.buildTree(cst, logger);

    ensure("built", ast);
    ensure("result", ast->main == builder.buildFunctionResult);
    ensure_equals("called", builder.buildFunctionCalled, 1);
    ensure("main", builder.function == &cst.main);
    ensure("logger", builder.logger == &logger);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    BuilderMock builder;
    
    error::Logger logger;
    cst::Tree cst;
    
    builder.buildFunctionSuccessful = false;

    boost::optional<ast::Tree> ast = builder.buildTree(cst, logger);
    
    ensure_not("not built", ast);
    ensure_equals("called", builder.buildFunctionCalled, 1);
    ensure("main", builder.function == &cst.main);
    ensure("logger", builder.logger == &logger);
}

}
