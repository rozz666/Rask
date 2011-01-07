// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_VARIABLEFACTORY_HPP
#define RASK_TEST_VARIABLEFACTORY_HPP

#include <rask/ast/Variable.hpp>

namespace rask
{

namespace test
{

class VariableFactory
{
public:

    ast::SharedVariable createShared(const std::string& name);
    ast::SharedVariable createShared(const cst::Identifier& name);
};

}

}

#endif // RASK_TEST_VARIABLEFACTORY_HPP
