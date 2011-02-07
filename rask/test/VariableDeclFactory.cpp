// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/VariableDeclFactory.hpp>
#include <rask/test/VariableFactory.hpp>

namespace rask
{

namespace test
{

ast::VariableDecl VariableDeclFactory::create(const std::string& name)
{
    ast::SharedVariable var(VariableFactory::createShared(name));
    return ast::VariableDecl(var, ast::Constant(0));
}

}
}
