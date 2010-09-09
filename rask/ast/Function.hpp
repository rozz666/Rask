// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTION_HPP
#define RASK_AST_FUNCTION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <rask/cst/Identifier.hpp>
#include <rask/ast/FunctionVisitor.hpp>

namespace rask
{
namespace ast
{

class Function
{
public:

    Function() { }
    virtual ~Function() { }
    
    virtual cst::Identifier name() const = 0;
    virtual unsigned short argCount() const = 0;
    virtual void accept(FunctionVisitor& visitor) = 0;

protected:

    Function(const Function& ) { }
    Function& operator=(const Function& ) { return *this; }
};

typedef boost::shared_ptr<Function> SharedFunction;
typedef boost::weak_ptr<Function> WeakFunction;

}
}
#endif // RASK_AST_FUNCTION_HPP
