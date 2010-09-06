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

    struct BuildFunctionDecl
    {
        int N;
        const rask::cst::Function *f;
        boost::optional<rask::ast::FunctionDecl> result;
    };

    struct BuildFunction
    {
        int N;
        const rask::cst::Function *f;
    };

    int counter;
    std::vector<BuildFunctionDecl> buildFunctionDeclCalls;
    std::vector<BuildFunction> buildFunctionCalls;
    bool buildFunctionDeclSuccessful;
    bool buildFunctionSuccessful;

    BuilderMock(rask::error::Logger& logger)
        : rask::ast::Builder(logger, st), counter(0), buildFunctionDeclSuccessful(true), buildFunctionSuccessful(true)
    {
    }

    virtual boost::optional<rask::ast::FunctionDecl> buildFunctionDecl(const rask::cst::Function& f)
    {
        BuildFunctionDecl bfd = { ++counter, &f };

        buildFunctionDeclCalls.push_back(bfd);
        
        if (!buildFunctionDeclSuccessful) return boost::none;

        rask::ast::FunctionDecl fd(f.name);

        buildFunctionDeclCalls.back().result = fd;
        
        return fd;
    }
    
    virtual bool buildFunction(const rask::cst::Function& f)
    {
        BuildFunction bf = { ++counter, &f };

        buildFunctionCalls.push_back(bf);

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
    ensure_equals("count", ast->functionCount(), 0u);
    ensure_equals("no calls 1", builder.buildFunctionDeclCalls.size(), 0u);
    ensure_equals("no calls 2", builder.buildFunctionCalls.size(), 0u);
}
    
template <>
template <>
void object::test<2>()
{
    using namespace rask;

    cst.functions.resize(2);
    
    boost::optional<ast::Tree> ast = builder.buildTree(cst);

    ensure("built", ast);
    ensure_equals("count", ast->functionCount(), 2u);
    ensure_equals("decl calls", builder.buildFunctionDeclCalls.size(), 2u);
    ensure_equals("calls", builder.buildFunctionCalls.size(), 2u);
    ensure_equals("decl called 1", builder.buildFunctionDeclCalls[0].N, 1);
    ensure_equals("decl called 2", builder.buildFunctionDeclCalls[1].N, 2);
    ensure_equals("called 1", builder.buildFunctionCalls[0].N, 3);
    ensure_equals("called 2", builder.buildFunctionCalls[1].N, 4);
    ensure("result", ast->function(0) == builder.buildFunctionDeclCalls[0].result->function());
    ensure("result", ast->function(1) == builder.buildFunctionDeclCalls[1].result->function());
    ensure("fd 1", builder.buildFunctionDeclCalls[0].f == &cst.functions[0]);
    ensure("f 1", builder.buildFunctionCalls[0].f == &cst.functions[0]);
    ensure("fd 2", builder.buildFunctionDeclCalls[1].f == &cst.functions[1]);
    ensure("f 2", builder.buildFunctionCalls[1].f == &cst.functions[1]);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cst.functions.resize(2);

    builder.buildFunctionDeclSuccessful = false;

    ensure_not("not built", builder.buildTree(cst));
    ensure_equals("decl calls", builder.buildFunctionDeclCalls.size(), 2u);
    ensure_equals("calls", builder.buildFunctionCalls.size(), 2u);
    ensure_equals("decl called 1", builder.buildFunctionDeclCalls[0].N, 1);
    ensure_equals("decl called 2", builder.buildFunctionDeclCalls[1].N, 2);
    ensure_equals("called 1", builder.buildFunctionCalls[0].N, 3);
    ensure_equals("called 2", builder.buildFunctionCalls[1].N, 4);
    ensure("fd 1", builder.buildFunctionDeclCalls[0].f == &cst.functions[0]);
    ensure("fd 2", builder.buildFunctionDeclCalls[1].f == &cst.functions[1]);
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cst.functions.resize(2);

    builder.buildFunctionSuccessful = false;
    
    ensure_not("not built", builder.buildTree(cst));
    ensure_equals("decl calls", builder.buildFunctionDeclCalls.size(), 2u);
    ensure_equals("calls", builder.buildFunctionCalls.size(), 2u);
    ensure_equals("decl called 1", builder.buildFunctionDeclCalls[0].N, 1);
    ensure_equals("decl called 2", builder.buildFunctionDeclCalls[1].N, 2);
    ensure_equals("called 1", builder.buildFunctionCalls[0].N, 3);
    ensure_equals("called 2", builder.buildFunctionCalls[1].N, 4);
    ensure("fd 1", builder.buildFunctionDeclCalls[0].f == &cst.functions[0]);
    ensure("f 1", builder.buildFunctionCalls[0].f == &cst.functions[0]);
    ensure("fd 2", builder.buildFunctionDeclCalls[1].f == &cst.functions[1]);
    ensure("f 2", builder.buildFunctionCalls[1].f == &cst.functions[1]);
}

}
