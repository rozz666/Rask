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
    virtual boost::optional<FunctionDecl> buildFunctionDecl(const cst::Function& f);
    virtual bool buildFunction(const cst::Function& f);
    virtual boost::optional<Tree> buildTree(const cst::Tree& cst);
    virtual boost::optional<VariableDecl> buildVariableDecl(const cst::VariableDecl& vd);
    virtual boost::optional<Expression> buildExpression(const cst::Expression& expr);
    virtual boost::optional<Expression> buildChainExpression(const cst::ChainExpression& expr);
    virtual boost::optional<Return> buildReturn(const cst::Return& ret);
    virtual boost::optional<FunctionCall> buildUnaryOperatorCall(const cst::UnaryOperatorCall& oc);

private:

    error::Logger& logger_;
    SymbolTable& symbolTable_;
    
    std::string functionSignature(const std::string& name, const std::vector<cst::ChainExpression>& args);
};

}
}

#endif // RASK_AST_BUILDER_HPP
