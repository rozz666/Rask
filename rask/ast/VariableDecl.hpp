// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_VARDECL_HPP
#define RASK_AST_VARDECL_HPP

#include <boost/cstdint.hpp>
#include <rask/ast/Variable.hpp>

namespace rask
{
namespace ast
{

class VariableDecl
{
public:

    VariableDecl(const cst::Identifier& name, boost::int32_t value) : var_(new Variable(name)), value_(value) { }
    
    SharedVariable var() const { return var_; }
    boost::int32_t value() const { return value_; }

private:

    SharedVariable var_;
    boost::int32_t value_;
};

inline bool operator==(const VariableDecl& left, const VariableDecl& right)
{
    return left.var() == right.var() && left.value() == right.value();
}

inline bool operator!=(const VariableDecl& left, const VariableDecl& right)
{
    return !(left == right);
}

}
}

#endif // RASK_AST_VARDECL_HPP
