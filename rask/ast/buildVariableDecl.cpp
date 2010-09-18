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

boost::optional<VariableDecl> Builder::buildVariableDecl(const cst::VariableDecl& vd)
{
    if (!vd.value)
    {
        logger_.log(error::Message::uninitializedVariable(vd.name.position, vd.name.value));
        return boost::none;
    }

    VariableDecl decl(vd.name, *buildExpression(*vd.value, symbolTable_));
    symbolTable_.add(decl.var());
    
    return decl;
}
   
    
}
}