// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>
#include <rask/Operators.hpp>

namespace rask
{
namespace ast
{

boost::optional<FunctionCall> Builder::buildUnaryOperatorCall(const cst::UnaryOperatorCall& oc, SharedScope scope)
{
    FunctionCall::Arguments args;
    args.push_back(*buildExpression(oc.expr, scope));
    return FunctionCall(*symbolTable_.getFunction(UNARY_MINUS_NAME), args);
}
    
}
}