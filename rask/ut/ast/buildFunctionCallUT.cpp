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
#include <rask/ast/BuiltinFunction.hpp>

namespace tut
{

struct buildFunctionCall_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::ast::SymbolTable st;
    rask::ast::Builder builder;
    rask::cst::FunctionCall ccall;
    
    buildFunctionCall_TestData() : file("test.rask"), builder(logger, st) { }
};

typedef test_group<buildFunctionCall_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.Builder.buildFunctionCall");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("f", 0));
    st.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    
    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall);
    
    ensure("built", call);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure("func", call->function().lock() == f);
    ensure_equals("no args", call->args().size(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("f", 2));
    st.add(f);
    ast::SharedVariable arg1(new ast::Variable(cst::Identifier::create(Position(file, 2, 10), "asia")));
    st.add(arg1);
    cst::Constant arg2 = cst::Constant::create(Position(file, 2, 20), 1);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    ccall.args.push_back(arg1->name());
    ccall.args.push_back(arg2);
    
    boost::optional<ast::FunctionCall> call = builder.buildFunctionCall(ccall);
    
    ensure("built", call);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure("func", call->function().lock() == f);
    ensure_equals("args", call->args().size(), 2u);
    ensure("args 1", getVariable(call->args()[0]).lock() == arg1);
    ensure_equals("args 2", getConstant(call->args()[1]), arg2.value);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
    
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "xxx");
    ccall.args.push_back(cst::Constant::create(Position(file, 2, 10), 1));
    
    ensure_not("not built", builder.buildFunctionCall(ccall));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::unknownIdentifier(ccall.function.position, ccall.function.value));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("abc", 1));
    st.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), f->name().value);
    
    ensure_not("not built", builder.buildFunctionCall(ccall));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(ccall.function.position, "abc()"));
}

template <>
template <>
void object::test<5>()
{
    using namespace rask;
    
    rask::ast::SharedBuiltinFunction f(new rask::ast::BuiltinFunction("print", 1));
    st.add(f);
    ccall.function = cst::Identifier::create(Position(file, 2, 4), "print");
    ccall.args.push_back(cst::Constant::create(Position(file, 2, 10), 1));
    ccall.args.push_back(cst::Constant::create(Position(file, 2, 20), 2));
    
    ensure_not("not built", builder.buildFunctionCall(ccall));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(ccall.function.position, "print(int, int)"));
}

}
