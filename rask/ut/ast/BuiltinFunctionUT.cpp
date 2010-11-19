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
#include <rask/ast/BuiltinFunction.hpp>

namespace
{

struct FunctionTestVisitor : public rask::ast::FunctionVisitor
{
    rask::ast::BuiltinFunction *f;

    FunctionTestVisitor() : f(0) { }

    virtual void visit(rask::ast::BuiltinFunction& f) { this->f = &f; }
    virtual void visit(rask::ast::CustomFunction& f) { }
};

}

namespace tut
{

struct BuiltinFunction_TestData
{
};

typedef test_group<BuiltinFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.BuiltinFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::BuiltinFunction bf1("test1", 2);
    ast::BuiltinFunction bf2("test2", 0);
    ast::Function& f1 = bf1;
    ast::Function& f2 = bf2;
    
    cst::Identifier name = f1.name();
    
    ENSURE(f1.name().position == Position());
    ENSURE_EQUALS(f1.name().value, "test1");
    ENSURE_EQUALS(f1.argCount(), 2u);
    ENSURE(f2.name().position == Position());
    ENSURE_EQUALS(f2.name().value, "test2");
    ENSURE_EQUALS(f2.argCount(), 0u);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::BuiltinFunction bf("xxx", 0);
    ast::Function& f = bf;
    FunctionTestVisitor v;

    f.accept(v);
    ENSURE(v.f == &bf);
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;

    ENSURE(ast::BuiltinFunction("xxx", ast::VOID, 0).type() == ast::VOID);
    ENSURE(ast::BuiltinFunction("xxx", ast::INT32, 0).type() == ast::INT32);
}

}
