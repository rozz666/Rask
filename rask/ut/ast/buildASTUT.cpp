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
#include <rask/ast/buildAST.hpp>

namespace tut
{

struct buildAST_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Tree cst;
    rask::cst::Function& cf;
    
    buildAST_TestData() : file("test.rask"), cf(cst.main)
    {
        cf.name.value = "main";
        cf.name.position = rask::Position(file, 1, 2);
    }
};

typedef test_group<buildAST_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.buildAST");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    cf.calls.resize(2);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);
    cf.calls[0].args.push_back(1);

    cf.calls[1].function.value = "print";
    cf.calls[1].function.position = Position(file, 3, 4);
    cf.calls[1].args.push_back(2);

    boost::optional<ast::Tree> tree = ast::buildAST(cst, logger);

    ensure("parsed", tree);
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("value count", tree->main.valueCount(), 2u);
    ensure_equals("value 1", tree->main.value(0), 1);
    ensure_equals("value 2", tree->main.value(1), 2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cf.calls.resize(1);
    cf.calls[0].function.value = "xxx";
    cf.calls[0].function.position = Position(file, 2, 4);

    ensure_not("not parsed", ast::buildAST(cst, logger));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::unknownIdentifier(cf.calls[0].function.position, cf.calls[0].function.value));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
   
    cf.calls.resize(1);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);

    ensure_not("not parsed", ast::buildAST(cst, logger));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(cf.calls[0].function.position, "print()"));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cf.calls.resize(1);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);
    cf.calls[0].args.push_back(1);
    cf.calls[0].args.push_back(2);

    ensure_not("not parsed", ast::buildAST(cst, logger));
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(cf.calls[0].function.position, "print(int, int)"));
}

}
