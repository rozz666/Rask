// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTIONVISITOR_HPP
#define RASK_AST_FUNCTIONVISITOR_HPP

namespace rask
{
namespace ast
{
        
class BuiltinFunction;
class CustomFunction;
    
class FunctionVisitor
{
public:
    
    FunctionVisitor() { }
    virtual ~FunctionVisitor() { }

    virtual void visit(BuiltinFunction& f) = 0;
    virtual void visit(CustomFunction& f) = 0;

protected:

    FunctionVisitor(const FunctionVisitor& ) { }
    FunctionVisitor& operator=(const FunctionVisitor& ) { return *this; }
};
    
}
}
#endif // RASK_AST_FUNCTIONVISITOR_HPP
