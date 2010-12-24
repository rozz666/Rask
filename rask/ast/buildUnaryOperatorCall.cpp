// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

boost::optional<FunctionCall> Builder::buildUnaryOperatorCall(const cst::UnaryOperatorCall& oc)
{
    FunctionCall::Arguments args;
    args.push_back(*buildUnaryExpression(oc.expr));
    return FunctionCall(*symbolTable_.getFunction("operator-"), args);
}
    
}
}