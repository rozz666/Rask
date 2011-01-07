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
#include <rask/test/VariableFactory.hpp>

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

ast::SharedCustomFunction FunctionFactory::createShared(
    const std::string& name, ast::BasicType type, short unsigned int numArgs)
{
    return createShared(name, type, std::vector<ast::BasicType>(numArgs, ast::INT32));
}

ast::SharedCustomFunction FunctionFactory::createShared(
    const std::string& name, ast::BasicType type, const std::vector<ast::BasicType>& args)
{
    ast::SharedCustomFunction f(
        new ast::CustomFunction(cst::Identifier::create(Position(), name), type));

    VariableFactory variableFactory;

    for (unsigned short i = 0; i != args.size(); ++i)
    {
        f->addArg(variableFactory.createShared("arg" + boost::lexical_cast<std::string>(i), args[i]));
    }

    return f;
}


}
}

