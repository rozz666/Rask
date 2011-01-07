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

boost::optional<Builder::Functions> Builder::buildFunctionDecls(const std::vector<cst::Function>& cfs, Tree& ast)
{
    bool failed = false;
    Functions functions;
    functions.reserve(cfs.size());

    BOOST_FOREACH(const cst::Function& f, cfs)
    {
        if (boost::optional<FunctionDecl> fd = buildFunctionDecl(f, variableFactory))
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

    return functions;
}

bool Builder::buildFunctions(const Builder::Functions& fs, SharedScopeFactory scopeFactory)
{
    bool failed = false;

    BOOST_FOREACH(const Functions::value_type& f, fs)
    {
        if (!buildFunction(*f.first, f.second, scopeFactory->createScope())) failed = true;
    }

    return !failed;
}

boost::optional<Tree> Builder::buildTree(const cst::Tree& cst, SharedScopeFactory scopeFactory)
{
    Tree ast;
    boost::optional<Functions> functions = buildFunctionDecls(cst.functions, ast);

    if (!functions || !buildFunctions(*functions, scopeFactory)) return boost::none;

    if (!functionTable_.getFunction("main"))
    {
        logger_.log(error::Message::missingMainFunction(Position(cst.end.file)));
        return boost::none;
    }

    return ast;
}

}
}