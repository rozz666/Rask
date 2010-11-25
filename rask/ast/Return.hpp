// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_RETURN_HPP
#define RASK_AST_RETURN_HPP

#include <rask/ast/Expression.hpp>

namespace rask {

namespace ast {

class Return
{
public:

    Return(const Expression& expr) : expr_(expr) { }

    const Expression& expr() const { return expr_; }

private:

    Expression expr_;
};

}

}

#endif // RASK_AST_RETURN_HPP
