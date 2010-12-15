// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/FunctionFactory.hpp>
#include <boost/lexical_cast.hpp>

namespace rask {

namespace test {

ast::CustomFunction FunctionFactory::create(const std::string& name)
{
    return ast::CustomFunction(cst::Identifier::create(Position(), name), ast::VOID);
}

ast::CustomFunction FunctionFactory::create(const std::string& name, ast::BasicType type)
{
    return ast::CustomFunction(cst::Identifier::create(Position(), name), type);
}

ast::SharedCustomFunction FunctionFactory::createShared(const std::string& name)
{
    return ast::SharedCustomFunction(
        new ast::CustomFunction(cst::Identifier::create(Position(), name), ast::VOID));
}

ast::SharedCustomFunction FunctionFactory::createShared(unsigned x, unsigned y, const std::string& name)
{
    return ast::SharedCustomFunction(
        new ast::CustomFunction(cst::Identifier::create(Position("", x, y), name), ast::VOID));
}

ast::SharedCustomFunction FunctionFactory::createShared(const std::string& name, ast::BasicType type, short unsigned int numArgs)
{
    ast::SharedCustomFunction f(
        new ast::CustomFunction(cst::Identifier::create(Position(), name), type));
    
    for (unsigned short i = 0; i != numArgs; ++i)
    {
        f->addArg(cst::Identifier::create(Position(), "arg" + boost::lexical_cast<std::string>(i)));
    }
    
    return f;
}


}
}

