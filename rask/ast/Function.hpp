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

namespace rask
{
namespace ast
{

class Function
{
public:

    std::size_t valueCount() const { return values.size(); }
    int value(std::size_t idx) const { return values[idx]; }
    void addValue(int value) { values.push_back(value); }

    friend bool operator==(const Function& left, const Function& right)
    {
        return left.values == right.values;
    }

    friend bool operator!=(const Function& left, const Function& right)
    {
        return !(left == right);
    }

private:
        
    std::vector<int> values;
};

}
}

#endif /* RASK_AST_FUNCTION_HPP */