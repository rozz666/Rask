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

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace ast
{

class VarDecl
{
public:

    VarDecl(const cst::Identifier& name, boost::int32_t value) : name_(name), value_(value) { }
    
    cst::Identifier name() const { return name_; }
    boost::int32_t value() const { return value_; }

private:

    cst::Identifier name_;
    boost::int32_t value_;
};

typedef boost::shared_ptr<VarDecl> SharedVarDecl;

}
}

#endif // RASK_AST_VARDECL_HPP
