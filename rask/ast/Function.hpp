// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTION_HPP
#define RASK_AST_FUNCTION_HPP

#include <cstddef>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <rask/ast/Statement.hpp>

namespace rask
{
namespace ast
{

class Function
{
public:

    Function(const cst::Identifier& name) : name_(name) { }

    cst::Identifier name() const { return name_; }
    
    const Statement& stmt(std::size_t idx) const { return stmts_[idx]; }
    std::size_t stmtCount() const { return stmts_.size(); }
    void addStmt(const Statement& stmt) { stmts_.push_back(stmt); }

    friend bool operator==(const Function& left, const Function& right)
    {
        return
            left.name_.position == right.name_.position &&
            left.name_.value == right.name_.value &&
            left.stmts_ == right.stmts_;
    }

    friend bool operator!=(const Function& left, const Function& right)
    {
        return !(left == right);
    }

private:

    cst::Identifier name_;
    std::vector<Statement> stmts_;
};

typedef boost::shared_ptr<Function> SharedFunction;
typedef boost::weak_ptr<Function> WeakFunction;

}
}

#endif /* RASK_AST_FUNCTION_HPP */