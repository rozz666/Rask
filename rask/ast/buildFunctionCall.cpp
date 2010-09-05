// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

struct MakeFunctionCall : boost::static_visitor<FunctionCall>
{
    SymbolTable& st;

    MakeFunctionCall(SymbolTable& st) : st(st) { }
    
    FunctionCall operator()(const cst::Constant& c)
    {
        return c.value;
    }

    FunctionCall operator()(const cst::Identifier& id)
    {
        return st.getVariable(id.value);
    }
};
    
boost::optional<FunctionCall> Builder::buildFunctionCall(const cst::FunctionCall& call)
{
    if (call.function.value != "print")
    {
        logger_.log(error::Message::unknownIdentifier(call.function.position, call.function.value));
        return boost::none;
    }

    if (call.args.size() != 1)
    {
        logger_.log(error::Message::functionNotFound(call.function.position, functionSignature("print", call.args)));
        return boost::none;
    }

    MakeFunctionCall mfc(symbolTable_);

    return call.args[0].apply_visitor(mfc);
}
    
}
}