// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_VARIABLE_HPP
#define RASK_AST_VARIABLE_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace ast
{

class Variable
{
public:

    Variable(const cst::Identifier& name) : name_(name) { }
    
    cst::Identifier name() const { return name_; }
    
private:
    
    cst::Identifier name_;
};

typedef boost::shared_ptr<Variable> SharedVariable;
typedef boost::weak_ptr<Variable> WeakVariable;

}
}

#endif