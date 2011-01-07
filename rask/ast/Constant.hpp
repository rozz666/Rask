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
    explicit Constant(boost::int32_t value) : value_(value), isBoolean_(false) { }
    explicit Constant(bool value) : value_(value), isBoolean_(true) { }

    friend bool operator==(const Constant& left, const Constant& right)
    {
        return left.isBoolean_ == right.isBoolean_ && left.value_ == right.value_;
    }

    boost::int32_t getInt32() const { return value_; }
    boost::int32_t getBoolean() const { return value_; }

private:

    boost::int32_t value_;
    bool isBoolean_;
};

inline bool operator!=(const Constant& left, const Constant& right)
{
    return !(left == right);
}

}
}
#endif // RASK_AST_CONSTANT_HPP