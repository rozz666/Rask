// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_VARIABLEDECLFACTORY_HPP
#define RASK_TEST_VARIABLEDECLFACTORY_HPP

#include <rask/ast/VariableDecl.hpp>

namespace rask
{

namespace test
{

class VariableDeclFactory
{
public:

    ast::VariableDecl create(const std::string& name);
};

}
}

#endif // RASK_TEST_VARIABLEDECLFACTORY_HPP
