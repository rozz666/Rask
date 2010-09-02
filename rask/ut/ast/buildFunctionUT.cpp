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

    std::vector<const rask::cst::VarDecl *> varDecls;
    bool buildVarDeclSuccessful;

    BuilderMock(rask::error::Logger& logger) : rask::ast::Builder(logger), buildVarDeclSuccessful(true) { }
    
    virtual boost::optional<rask::ast::VarDecl> buildVarDecl(const rask::cst::VarDecl& vd)
    {
        varDecls.push_back(&vd);

        if (!buildVarDeclSuccessful) return boost::none;
        
        return rask::ast::VarDecl(vd.name, 0);
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
    rask::ast::Builder builder;
    
    buildFunctionAST_TestData() : file("test.rask"), builder(logger)
    {
        cf.name.value = "main";
        cf.name.position = rask::Position(file, 1, 2);
    }

    rask::cst::Constant makeConstant(rask::Position pos, boost::int32_t val)
    {
        rask::cst::Constant c;
        c.position = pos;
        c.value = val;
        return c;
    }

    const rask::ast::VarDecl& getVD(const rask::ast::Statement& stmt)
    {
        return boost::get<rask::ast::VarDecl>(stmt);
    }

    const rask::ast::FunctionCall& getFCall(const rask::ast::Statement& stmt)
    {
        return boost::get<rask::ast::FunctionCall>(stmt);
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
    
    cf.stmts.resize(2, cst::FunctionCall());
    cst::FunctionCall& call1 = boost::get<cst::FunctionCall>(cf.stmts[0]);
    cst::FunctionCall& call2 = boost::get<cst::FunctionCall>(cf.stmts[1]);
    
    call1.function.value = "print";
    call1.function.position = Position(file, 2, 4);
    cst::Constant arg1 = makeConstant(Position(file, 2, 10), 1);
    call1.args.push_back(arg1);

    call2.function.value = "print";
    call2.function.position = Position(file, 3, 4);
    cst::Constant arg2 = makeConstant(Position(file, 3, 10), 2);
    call2.args.push_back(arg2);

    boost::optional<ast::Function> f = builder.buildFunction(cf);

    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("value count", f->stmtCount(), 2u);
    ensure_equals("value 1", getFCall(f->stmt(0)), arg1.value);
    ensure_equals("value 2", getFCall(f->stmt(1)), arg2.value);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cf.stmts.push_back(cst::FunctionCall());
    cst::FunctionCall& call1 = boost::get<cst::FunctionCall>(cf.stmts[0]);
    call1.function.value = "xxx";
    call1.function.position = Position(file, 2, 4);

    ensure_not("not built", builder.buildFunction(cf));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::unknownIdentifier(call1.function.position, call1.function.value));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
   
    cf.stmts.push_back(cst::FunctionCall());
    cst::FunctionCall& call1 = boost::get<cst::FunctionCall>(cf.stmts[0]);
    call1.function.value = "print";
    call1.function.position = Position(file, 2, 4);

    ensure_not("not built", builder.buildFunction(cf));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(call1.function.position, "print()"));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cf.stmts.push_back(cst::FunctionCall());
    cst::FunctionCall& call1 = boost::get<cst::FunctionCall>(cf.stmts[0]);
    call1.function.value = "print";
    call1.function.position = Position(file, 2, 4);
    call1.args.push_back(makeConstant(Position(file, 2, 10), 1));
    call1.args.push_back(makeConstant(Position(file, 2, 13), 2));

    ensure_not("not built", builder.buildFunction(cf));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(call1.function.position, "print(int, int)"));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;

    cst::VarDecl vd1;
    vd1.name.value = "a";
    cf.stmts.push_back(vd1);
    cst::VarDecl vd2;
    vd2.name.value = "b";
    cf.stmts.push_back(vd2);

    BuilderMock builder(logger);
    
    boost::optional<ast::Function> f = builder.buildFunction(cf);
    
    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("2 decls", builder.varDecls.size(), 2u);
    ensure("decl 1", builder.varDecls[0] == &boost::get<cst::VarDecl>(cf.stmts[0]));
    ensure("decl 2", builder.varDecls[1] == &boost::get<cst::VarDecl>(cf.stmts[1]));
    ensure_equals("stmt count", f->stmtCount(), 2u);
    ensure_equals("stmt 1", getVD(f->stmt(0)).var()->name().value, vd1.name.value);
    ensure_equals("stmt 2", getVD(f->stmt(1)).var()->name().value, vd2.name.value);
}

template <>
template <>
void object::test<6>()
{
    using namespace rask;
    
    cf.stmts.push_back(cst::VarDecl());
    
    BuilderMock builder(logger);
    builder.buildVarDeclSuccessful = false;
    
    boost::optional<ast::Function> f = builder.buildFunction(cf);
    
    ensure_not("not built", f);
    ensure_equals("1 decl", builder.varDecls.size(), 1u);
    ensure("decl", builder.varDecls[0] == &boost::get<cst::VarDecl>(cf.stmts[0]));
}

}