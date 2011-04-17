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

#include <rask/ast/Variable.hpp>
#include <gmock/gmock.h>

namespace rask
{
namespace ast
{
namespace test
{

struct VariableFactoryMock : VariableFactory
{
    MOCK_METHOD2(createVariable, SharedVariable(const cst::Identifier&, BasicType));
};

typedef boost::shared_ptr<VariableFactoryMock> SharedVariableFactoryMock;

}
}
}

#endif // RASK_AST_TEST_VARIABLEFACTORYMOCK_HPP
