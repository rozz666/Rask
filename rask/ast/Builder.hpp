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
#include <rask/ast/FunctionTable.hpp>
#include <rask/ast/Scope.hpp>
#include <rask/cst/Tree.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace ast
{

class Builder
{
public:

    Builder(error::Logger& logger, FunctionTable& functionTable)
        : logger_(logger), functionTable_(functionTable) { }

    virtual boost::optional<FunctionCall> buildFunctionCall(const cst::FunctionCall& fc, SharedScope scope);
    virtual boost::optional<FunctionDecl> buildFunctionDecl(const cst::Function& f);
    virtual bool buildFunction(const cst::Function& cf, SharedCustomFunction f, SharedScope scope);
    virtual boost::optional<Tree> buildTree(const cst::Tree& cst, SharedScopeFactory scopeFactory);
    virtual boost::optional<VariableDecl> buildVariableDecl(const cst::VariableDecl& vd, SharedScope scope);
    virtual boost::optional<Expression> buildExpression(const cst::Expression& expr, SharedScope scope);
    virtual boost::optional<Expression> buildChainExpression(const cst::ChainExpression& expr, SharedScope scope);
    virtual boost::optional<Return> buildReturn(const cst::Return& ret, SharedScope scope);
    virtual boost::optional<FunctionCall> buildUnaryOperatorCall(const cst::UnaryOperatorCall& oc, SharedScope scope);

private:

    error::Logger& logger_;
    FunctionTable& functionTable_;

    std::string functionSignature(const std::string& name, const std::vector<cst::Expression>& args);
};

}
}

#endif // RASK_AST_BUILDER_HPP
