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
#include <rask/ast/Statement.hpp>

namespace rask
{
namespace ast
{

class Function
{
public:

    const Statement& stmt(std::size_t idx) const { return stmts_[idx]; }
    std::size_t stmtCount() const { return stmts_.size(); }
    void addStmt(const Statement& stmt) { stmts_.push_back(stmt); }

    friend bool operator==(const Function& left, const Function& right)
    {
        return left.stmts_ == right.stmts_;
    }

    friend bool operator!=(const Function& left, const Function& right)
    {
        return !(left == right);
    }

private:
        
    std::vector<Statement> stmts_;
};

}
}

#endif /* RASK_AST_FUNCTION_HPP */