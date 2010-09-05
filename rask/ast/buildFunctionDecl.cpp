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

boost::optional<FunctionDecl> Builder::buildFunctionDecl(const cst::Function& f)
{
    FunctionDecl fd(f.name);

    SharedFunction r = symbolTable_.add(fd.function());

    if (r != fd.function())
    {
        logger_.log(error::Message::redefinition(f.name.position, f.name.value + "()"));
        logger_.log(error::Message::previousDefinition(r->name().position, r->name().value + "()"));
        return boost::none;
    }
    
    return fd;
}

}
}