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
#include <rask/cst/Tree.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace ast
{

class Builder
{
public:

    Builder(error::Logger& logger) : logger_(logger) { }
    
    virtual boost::optional<FunctionCall> buildFunctionCall(const cst::FunctionCall& fc);
    virtual boost::optional<Function> buildFunction(const cst::Function& f);
    virtual boost::optional<Tree> buildTree(const cst::Tree& cst);
    virtual SharedVarDecl buildVarDecl(const cst::VarDecl& vd);

private:

    error::Logger& logger_;

    std::string functionSignature(const std::string& name, const std::vector<cst::Expression>& args);
};

}
}

#endif // RASK_AST_BUILDER_HPP
