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
#include <rask/ast/BasicType.hpp>

namespace rask
{
namespace ast
{

class Constant
{
public:

    Constant() { }
    explicit Constant(boost::int32_t value) : value_(value), type_(ast::INT32) { }
    explicit Constant(bool value) : value_(value), type_(ast::BOOLEAN) { }

    friend bool operator==(const Constant& left, const Constant& right)
    {
        return left.type_ == right.type_ && left.value_ == right.value_;
    }

    boost::int32_t getInt32() const { return value_; }
    boost::int32_t getBoolean() const { return value_; }

    BasicType type() const { return type_; }

private:

    boost::int32_t value_;
    BasicType type_;
};

inline bool operator!=(const Constant& left, const Constant& right)
{
    return !(left == right);
}

}
}
#endif // RASK_AST_CONSTANT_HPP