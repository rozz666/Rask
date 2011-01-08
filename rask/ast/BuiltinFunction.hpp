// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_BUILTINFUNCTION_HPP
#define RASK_AST_BUILTINFUNCTION_HPP

#include <string>
#include <rask/ast/Function.hpp>

namespace rask
{
namespace ast
{

class BuiltinFunction : public Function
{
public:

    BuiltinFunction(const std::string name, BasicType type, unsigned short argCount)
        : name_(name), type_(type), argCount_(argCount) { }

    virtual cst::Identifier name() const { return cst::Identifier::create(Position(), name_); }
    virtual unsigned short argCount() const { return argCount_; }
    virtual BasicType type() const { return type_; }
    virtual ArgumentTypes argTypes() const { return ArgumentTypes(); }

private:

    std::string name_;
    BasicType type_;
    unsigned short argCount_;
};

typedef boost::shared_ptr<BuiltinFunction> SharedBuiltinFunction;
typedef boost::weak_ptr<BuiltinFunction> WeakBuiltinFunction;

}
}

#endif // RASK_AST_BUILTINFUNCTION_HPP
