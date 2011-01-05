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

boost::optional<FunctionDecl> Builder::buildFunctionDecl(const cst::Function& f)
{
    FunctionDecl fd(f.name, f.type.value == "int32" ? INT32 : (f.type.value == "boolean" ? BOOLEAN : VOID));
    SharedCustomFunction cf = fd.function();
    
    SharedFunction r = symbolTable_.add(cf);

    if (r != cf)
    {
        logger_.log(error::Message::redefinition(f.name.position, f.name.value + "()"));
        logger_.log(error::Message::previousDefinition(r->name().position, r->name().value + "()"));
        return boost::none;
    }

    BOOST_FOREACH(const cst::FunctionArgument& arg, f.args)
    {
        cf->addArg(arg.name);
    }
    
    return fd;
}

}
}