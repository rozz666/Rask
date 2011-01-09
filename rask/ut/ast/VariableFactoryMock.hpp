// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_TEST_VARIABLEFACTORYMOCK_HPP
#define RASK_AST_TEST_VARIABLEFACTORYMOCK_HPP

#include <rask/test/Mock.hpp>
#include <rask/ast/Variable.hpp>

namespace rask
{
namespace ast
{
namespace test
{

CLASS_MOCK(VariableFactoryMock, VariableFactory)
{
    MOCK_METHOD(SharedVariable, createVariable, (const cst::Identifier&, name)(BasicType, type))
};

}
}
}

#endif // RASK_AST_TEST_VARIABLEFACTORYMOCK_HPP
