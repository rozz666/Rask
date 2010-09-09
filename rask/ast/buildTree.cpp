// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/foreach.hpp>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

boost::optional<Tree> Builder::buildTree(const cst::Tree& cst)
{
    Tree ast;
    bool failed = false;
    
    BOOST_FOREACH(const cst::Function& f, cst.functions)
    {
        if (boost::optional<FunctionDecl> fd = buildFunctionDecl(f))
        {
            ast.add(fd->function());
        }
        else
        {
            failed = true;
        }
    }
    
    BOOST_FOREACH(const cst::Function& f, cst.functions)
    {
        if (!buildFunction(f)) failed = true;
    }

    if (failed) return boost::none;

    try
    {
        symbolTable_.getFunction("main");
    }
    catch (const SymbolTableError& )
    {
        logger_.log(error::Message::missingMainFunction(Position(cst.end.file)));
        return boost::none;
    }
    
    return ast;
}  

}
}