// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/foreach.hpp>
#include <vector>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

boost::optional<Tree> Builder::buildTree(const cst::Tree& cst, SharedScopeFactory scopeFactory)
{
    Tree ast;
    bool failed = false;
    typedef std::vector<std::pair<const cst::Function *, SharedCustomFunction> > Functions;
    Functions functions;
    functions.reserve(cst.functions.size());

    BOOST_FOREACH(const cst::Function& f, cst.functions)
    {
        if (boost::optional<FunctionDecl> fd = buildFunctionDecl(f))
        {
            ast.add(fd->function());
            functions.push_back(Functions::value_type(&f, fd->function()));
        }
        else
        {
            failed = true;
        }
    }

    if (failed) return boost::none;

    BOOST_FOREACH(const Functions::value_type& f, functions)
    {
        if (!buildFunction(*f.first, f.second, scopeFactory->createScope())) failed = true;
    }

    if (failed) return boost::none;

    if (!functionTable_.getFunction("main"))
    {
        logger_.log(error::Message::missingMainFunction(Position(cst.end.file)));
        return boost::none;
    }

    return ast;
}

}
}