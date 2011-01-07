// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_FUNCTIONTABLE_HPP
#define RASK_AST_FUNCTIONTABLE_HPP

#include <map>
#include <vector>
#include <boost/optional.hpp>
#include <rask/cst/Identifier.hpp>
#include <rask/ast/Variable.hpp>
#include <rask/ast/CustomFunction.hpp>

namespace rask
{
namespace ast
{

class FunctionFamily
{
public:

    FunctionFamily(const std::string& name) : name_(name) { }

    SharedFunction addFunction(SharedFunction f)
    {
        return functions_.insert(Functions::value_type(f->argTypes(), f)).first->second;
    }

    boost::optional<SharedFunction> getFunction(const Function::ArgumentTypes& args)
    {
        Functions::const_iterator it = functions_.find(args);

        if (it == functions_.end()) return boost::none;

        return it->second;
    }

    SharedFunction getFirstFunction()
    {
        return functions_.begin()->second;
    }

    const std::string& name() const { return name_; }

private:

    typedef std::map<Function::ArgumentTypes, SharedFunction> Functions;

    std::string name_;
    Functions functions_;
};

typedef boost::shared_ptr<FunctionFamily> SharedFunctionFamily;

class FunctionTable
{
public:

    SharedFunction add(SharedFunction function)
    {
        Families::iterator fam = families_.find(function->name().value);

        if (fam == families_.end())
        {
            SharedFunctionFamily newFamily(new FunctionFamily(function->name().value));
            fam = families_.insert(Families::value_type(newFamily->name(), newFamily)).first;
        }

        return fam->second->addFunction(function);
    }

    boost::optional<SharedFunction> getFunction(const std::string& name) const
    {
        typename Families::const_iterator it = families_.find(name);

        if (it == families_.end()) return boost::none;

        return it->second->getFirstFunction();
    }

    boost::optional<SharedFunctionFamily> getFamily(const std::string& name) const
    {
        Families::const_iterator it = families_.find(name);

        if (it == families_.end()) return boost::none;

        return it->second;
    }

private:

    typedef std::map<std::string, SharedFunctionFamily> Families;

    Families families_;
};

}
}

#endif // RASK_AST_FUNCTIONTABLE_HPP
