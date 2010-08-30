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

struct buildFunctionAST_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Function cf;
    rask::ast::Builder builder;
    
    buildFunctionAST_TestData() : file("test.rask")
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

    boost::optional<ast::Function> f = builder.buildFunction(cf, logger);

    ensure("built", f);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("value count", f->valueCount(), 2u);
    ensure_equals("value 1", f->value(0), arg1.value);
    ensure_equals("value 2", f->value(1), arg2.value);
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

    ensure_not("not built", builder.buildFunction(cf, logger));
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

    ensure_not("not built", builder.buildFunction(cf, logger));
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

    ensure_not("not built", builder.buildFunction(cf, logger));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(call1.function.position, "print(int, int)"));
}

}
