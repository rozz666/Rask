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

boost::optional<VarDecl> Builder::buildVarDecl(const cst::VarDecl& vd)
{
    if (!vd.value)
    {
        logger_.log(error::Message::uninitializedVariable(vd.name.position, vd.name.value));
        return boost::none;
    }
    
    VarDecl decl(vd.name, vd.value->value);
    symbolTable_.add(decl.var());
    
    return decl;
}
   
    
}
}