// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_SCOPE_HPP
#define RASK_AST_SCOPE_HPP

#include <boost/shared_ptr.hpp>

namespace rask
{

namespace ast
{

class Scope
{
};

typedef boost::shared_ptr<Scope> SharedScope;

}

}

#endif // RASK_AST_SCOPE_HPP
