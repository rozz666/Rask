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

boost::optional<Tree> Builder::buildTree(const cst::Tree& cst)
{
    boost::optional<FunctionDecl> fd = buildFunctionDecl(cst.functions[0]);

    if (!fd) return boost::none;
    
    if (!buildFunction(cst.functions[0])) return boost::none;
    
    Tree ast;
    ast.main = fd->function();
    
    return ast;
}  

}
}