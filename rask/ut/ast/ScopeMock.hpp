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

#include <boost/shared_ptr.hpp>
#include <rask/ast/Scope.hpp>
#include <rask/test/Mock.hpp>

namespace rask
{
namespace ast
{
namespace test
{

CLASS_MOCK(ScopeMock, Scope)
{
    MOCK_METHOD(SharedVariable, addVariable, (SharedVariable, var))
    MOCK_METHOD(boost::optional<SharedVariable>, getVariable, (const std::string&, name))
};

typedef boost::shared_ptr<ScopeMock> SharedScopeMock;

}
}
}



#endif // RASK_AST_TEST_SCOPEMOCK_HPP
