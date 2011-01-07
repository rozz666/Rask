// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_CUSTOMFUNCTION_HPP
#define RASK_AST_CUSTOMFUNCTION_HPP

#include <cstddef>
#include <vector>
#include <rask/ast/Function.hpp>
#include <rask/ast/Statement.hpp>

namespace rask
{
namespace ast
{

class CustomFunction : public Function
{
public:

    CustomFunction(const cst::Identifier& name, BasicType type)
        : name_(name), type_(type) { }

    virtual cst::Identifier name() const { return name_; }
    virtual unsigned short argCount() const { return args_.size(); }
    virtual void accept(FunctionVisitor& visitor) { visitor.visit(*this); }
    virtual BasicType type() const { return type_; }

    const Statement& stmt(std::size_t idx) const { return stmts_[idx]; }
    std::size_t stmtCount() const { return stmts_.size(); }
    void addStmt(const Statement& stmt) { stmts_.push_back(stmt); }

    void addArg(SharedVariable arg)
    {
        args_.push_back(arg);
    }

    SharedVariable arg(unsigned short index) const { return args_[index]; }

private:

    cst::Identifier name_;
    BasicType type_;
    std::vector<Statement> stmts_;
    std::vector<SharedVariable> args_;
};

typedef boost::shared_ptr<CustomFunction> SharedCustomFunction;
typedef boost::weak_ptr<CustomFunction> WeakCustomFunction;

}
}

#endif /* RASK_AST_CUSTOMFUNCTION_HPP */