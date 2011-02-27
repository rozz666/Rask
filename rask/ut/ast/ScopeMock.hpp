// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_TEST_SCOPEMOCK_HPP
#define RASK_AST_TEST_SCOPEMOCK_HPP

#include <rask/ast/Scope.hpp>
#include <gmock/gmock.h>

namespace rask
{
namespace ast
{
namespace test
{

struct ScopeMock : Scope
{
    MOCK_METHOD1(addVariable, SharedVariable(SharedVariable));
    MOCK_METHOD1(getVariable, boost::optional<SharedVariable>(const std::string&));
};

typedef boost::shared_ptr<ScopeMock> SharedScopeMock;

}
}
}



#endif // RASK_AST_TEST_SCOPEMOCK_HPP
