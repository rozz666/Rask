// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_CONSTANT_HPP
#define RASK_AST_CONSTANT_HPP

#include <boost/cstdint.hpp>

namespace rask
{
namespace ast
{

class Constant
{
public:

    Constant() { }
    Constant(boost::int32_t value) : value_(value) { }
    operator boost::int32_t() const { return value_; }

    friend bool operator==(const Constant& left, const Constant& right)
    {
        return left.value_ == right.value_;
    }

private:

    boost::int32_t value_;
};

inline bool operator!=(const Constant& left, const Constant& right)
{
    return !(left == right);
}

}
}
#endif // RASK_AST_CONSTANT_HPP