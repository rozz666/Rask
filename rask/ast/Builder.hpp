// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_BUILDER_HPP
#define RASK_AST_BUILDER_HPP

#include <boost/optional.hpp>
#include <rask/ast/Tree.hpp>
#include <rask/ast/SymbolTable.hpp>
#include <rask/cst/Tree.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace ast
{

class Builder
{
public:

    Builder(error::Logger& logger, SymbolTable& symbolTable) : logger_(logger), symbolTable_(symbolTable) { }
    
    virtual boost::optional<FunctionCall> buildFunctionCall(const cst::FunctionCall& fc);
    virtual boost::optional<Function> buildFunction(const cst::Function& f);
    virtual boost::optional<Tree> buildTree(const cst::Tree& cst);
    virtual boost::optional<VariableDecl> buildVariableDecl(const cst::VariableDecl& vd);

private:

    error::Logger& logger_;
    SymbolTable& symbolTable_;
    
    std::string functionSignature(const std::string& name, const std::vector<cst::Expression>& args);
};

}
}

#endif // RASK_AST_BUILDER_HPP
