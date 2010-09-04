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

    struct BuildVariableDecl
    {
        int N;
        const rask::cst::VariableDecl *vd;
    };
    
    struct BuildFunctionCall
    {
        int N;
        const rask::cst::FunctionCall *fc;
    };
    
    int counter;
    std::vector<BuildVariableDecl> buildVariableDeclCalls;
    std::vector<BuildFunctionCall> buildFunctionCallCalls;
    bool buildVariableDeclSuccessful;
    bool buildFunctionCallSuccessful;

    BuilderMock(rask::error::Logger& logger, rask::ast::SymbolTable st)
        : rask::ast::Builder(logger, st), counter(0), buildVariableDeclSuccessful(true), buildFunctionCallSuccessful(true) { }
    
    virtual boost::optional<rask::ast::VariableDecl> buildVariableDecl(const rask::cst::VariableDecl& vd)
    {
        BuildVariableDecl bvd = { ++counter, &vd };
        buildVariableDeclCalls.push_back(bvd);

        if (!buildVariableDeclSuccessful) return boost::none;
        
        return rask::ast::VariableDecl(vd.name, 0);
    }

    virtual boost::optional<rask::ast::FunctionCall> buildFunctionCall(const rask::cst::FunctionCall& fc)
    {
        BuildFunctionCall bfc = { ++counter, &fc };
        buildFunctionCallCalls.push_back(bfc);

        if (!buildFunctionCallSuccessful) return boost::none;

        return rask::ast::FunctionCall(boost::get<rask::cst::Constant>(fc.args[0]).value);
    }
};
    
}

namespace tut
{

struct buildFunctionAST_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Function cf;
    rask::ast::SymbolTable st;
    BuilderMock builder;
    
    buildFunctionAST_TestData() : file("test.rask"), builder(logger, st)
    {
        cf.name.value = "main";
        cf.name.position = rask::Position(file, 1, 2);
    }
};

typedef test_group<buildFunctionAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    boost::optional<ast::Function> f = builder.buildFunction(cf);

    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("no stmts", f->stmtCount(), 0u);
}
        
        
template <>
template <>
void object::test<2>()
{
    using namespace rask;

    boost::int32_t c1 = 1;
    boost::int32_t c2 = 2;
    
    cf.stmts.resize(2, cst::FunctionCall());
    getFunctionCall(cf.stmts[0]).args.push_back(cst::Constant::create(Position(), c1));
    getFunctionCall(cf.stmts[1]).args.push_back(cst::Constant::create(Position(), c2));

    boost::optional<ast::Function> f = builder.buildFunction(cf);

    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("count", f->stmtCount(), 2u);
    ensure_equals("fcall 1", getInt32(getFunctionCall(f->stmt(0))), c1);
    ensure_equals("fcall 2", getInt32(getFunctionCall(f->stmt(1))), c2);
    ensure_equals("buildFC #", builder.buildFunctionCallCalls.size(), 2u);
    ensure_equals("buildFC 1", builder.buildFunctionCallCalls[0].N, 1);
    ensure("buildFC 1 fc", builder.buildFunctionCallCalls[0].fc == &getFunctionCall(cf.stmts[0]));
    ensure_equals("buildFC 2", builder.buildFunctionCallCalls[1].N, 2);
    ensure("buildFC 2 fc", builder.buildFunctionCallCalls[1].fc == &getFunctionCall(cf.stmts[1]));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    cf.stmts.push_back(cst::FunctionCall());
    cf.stmts.push_back(cst::FunctionCall());
    
    builder.buildFunctionCallSuccessful = false;
    
    boost::optional<ast::Function> f = builder.buildFunction(cf);
    
    ensure_not("not built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("buildFC #", builder.buildFunctionCallCalls.size(), 1u);
    ensure_equals("buildFC 1", builder.buildFunctionCallCalls[0].N, 1);
    ensure("buildFC 1 fc", builder.buildFunctionCallCalls[0].fc == &getFunctionCall(cf.stmts[0]));
}


template <>
template <>
void object::test<4>()
{
    using namespace rask;

    cst::VariableDecl vd1;
    vd1.name.value = "a";
    cf.stmts.push_back(vd1);
    cst::VariableDecl vd2;
    vd2.name.value = "b";
    cf.stmts.push_back(vd2);

    boost::optional<ast::Function> f = builder.buildFunction(cf);
    
    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("2 decls", builder.buildVariableDeclCalls.size(), 2u);
    ensure_equals("decl 1", builder.buildVariableDeclCalls[0].N, 1);
    ensure("decl 1 vd", builder.buildVariableDeclCalls[0].vd == &getVariableDecl(cf.stmts[0]));
    ensure_equals("decl 2", builder.buildVariableDeclCalls[1].N, 2);
    ensure("decl 2 vd", builder.buildVariableDeclCalls[1].vd == &getVariableDecl(cf.stmts[1]));
    ensure_equals("stmt count", f->stmtCount(), 2u);
    ensure_equals("stmt 1", getVariableDecl(f->stmt(0)).var()->name().value, vd1.name.value);
    ensure_equals("stmt 2", getVariableDecl(f->stmt(1)).var()->name().value, vd2.name.value);
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    
    cf.stmts.push_back(cst::VariableDecl());
    cf.stmts.push_back(cst::VariableDecl());
    
    builder.buildVariableDeclSuccessful = false;
    
    boost::optional<ast::Function> f = builder.buildFunction(cf);
    
    ensure_not("not built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("1 decl", builder.buildVariableDeclCalls.size(), 1u);
    ensure_equals("decl 1", builder.buildVariableDeclCalls[0].N, 1);
    ensure("decl 1 vd", builder.buildVariableDeclCalls[0].vd == &boost::get<cst::VariableDecl>(cf.stmts[0]));
}

}