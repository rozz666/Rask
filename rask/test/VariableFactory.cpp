// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/VariableFactory.hpp>

namespace rask
{
namespace test
{

ast::SharedVariable VariableFactory::createShared(const std::string& name, rask::ast::BasicType type)
{
    return ast::SharedVariable(new ast::Variable(cst::Identifier::create(Position(), name), type));
}

ast::SharedVariable VariableFactory::createShared(const rask::cst::Identifier& name)
{
    return ast::SharedVariable(new ast::Variable(name, ast::INT32));
}

}
}

