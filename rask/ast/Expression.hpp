// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_EXPRESSION_HPP
#define RASK_AST_EXPRESSION_HPP

#include <vector>
#include <boost/variant.hpp>
#include <rask/ast/Function.hpp>
#include <rask/ast/Constant.hpp>
#include <rask/ast/Variable.hpp>

namespace rask
{
namespace ast
{

class FunctionCall;

typedef boost::variant<Constant, WeakVariable, boost::recursive_wrapper<FunctionCall> > Expression;

class FunctionCall
{
public:

    typedef std::vector<Expression> Arguments;

    FunctionCall(WeakFunction function, const Arguments& args) : function_(function), args_(args) { }

    WeakFunction function() const { return function_; }
    const Arguments& args() const { return args_; }

private:

    WeakFunction function_;
    Arguments args_;
};


inline Constant& getConstant(Expression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const Expression& e) { return boost::get<Constant>(e); }

inline WeakVariable& getVariable(Expression& e) { return boost::get<WeakVariable>(e); }
inline const WeakVariable& getVariable(const Expression& e) { return boost::get<WeakVariable>(e); }

inline FunctionCall& getFunctionCall(Expression& e) { return boost::get<FunctionCall>(e); }
inline const FunctionCall& getFunctionCall(const Expression& e) { return boost::get<FunctionCall>(e); }

BasicType getExpressionType(const Expression& e);

}

}

#endif // RASK_AST_EXPRESSION_HPP
