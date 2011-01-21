// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_FUNCTIONFACTORY_HPP
#define RASK_TEST_FUNCTIONFACTORY_HPP

#include <string>
#include <rask/ast/CustomFunction.hpp>

namespace rask {

namespace test {

class FunctionFactory
{
public:

    ast::CustomFunction create(const std::string& name);
    ast::CustomFunction create(const std::string& name, ast::BasicType type);
    ast::SharedCustomFunction createShared(const std::string& name);
    ast::SharedCustomFunction createShared(unsigned x, unsigned y, const std::string& name);
    ast::SharedCustomFunction createShared(const std::string& name, ast::BasicType type, unsigned short numArgs = 0);
    ast::SharedCustomFunction createShared(
        const std::string& name, ast::BasicType type, const std::vector<ast::BasicType>& args);
};

}

}

#endif // RASK_TEST_FUNCTIONFACTORY_HPP
