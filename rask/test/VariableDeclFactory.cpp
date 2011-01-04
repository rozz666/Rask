// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/VariableDeclFactory.hpp>

namespace rask
{

namespace test
{

ast::VariableDecl VariableDeclFactory::create(const std::string& name, boost::int32_t value)
{
    return ast::VariableDecl(cst::Identifier::create(Position(), name), ast::Constant(value));
}

}
}
