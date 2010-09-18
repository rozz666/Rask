// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <boost/foreach.hpp>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

boost::optional<FunctionCall> Builder::buildFunctionCall(const cst::FunctionCall& call)
{
    try
    {
        SharedFunction f = symbolTable_.getFunction(call.function.value);

        if (call.args.size() != f->argCount())
        {
            logger_.log(error::Message::functionNotFound(call.function.position, functionSignature(f->name().value, call.args)));
            return boost::none;
        }

        FunctionCall::Arguments args;

        BOOST_FOREACH(const cst::Expression& e, call.args)
        {
            args.push_back(*buildExpression(e, symbolTable_));
        }

        return FunctionCall(f, args);
    }
    catch (const SymbolTableError& )
    {
        logger_.log(error::Message::unknownIdentifier(call.function.position, call.function.value));
        return boost::none;
    }
}
    
}
}