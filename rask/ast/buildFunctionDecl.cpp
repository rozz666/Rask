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
    FunctionDecl fd(f.name);
    SharedCustomFunction cf = fd.function();
    
    SharedFunction r = symbolTable_.add(cf);

    if (r != cf)
    {
        logger_.log(error::Message::redefinition(f.name.position, f.name.value + "()"));
        logger_.log(error::Message::previousDefinition(r->name().position, r->name().value + "()"));
        return boost::none;
    }

    BOOST_FOREACH(const cst::Identifier& arg, f.args)
    {
        cf->addArg(arg);
        symbolTable_.add(cf->arg(cf->argCount() - 1));
    }
    
    return fd;
}

}
}