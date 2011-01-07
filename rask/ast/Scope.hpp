// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_SCOPE_HPP
#define RASK_AST_SCOPE_HPP

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <map>
#include <rask/ast/Variable.hpp>

namespace rask
{

namespace ast
{

class Scope
{
public:

    virtual SharedVariable addVariable(SharedVariable var)
    {
        return vars_.insert(Vars::value_type(var->name().value, var)).first->second;
    }

    virtual boost::optional<SharedVariable> getVariable(const std::string& name)
    {
        Vars::const_iterator it = vars_.find(name);

        if (it == vars_.end()) return boost::none;

        return it->second;
    }

private:

    typedef std::map<std::string, SharedVariable> Vars;
    Vars vars_;
};

typedef boost::shared_ptr<Scope> SharedScope;

class ScopeFactory
{
public:

    virtual SharedScope createScope() { return SharedScope(new Scope); }
};

typedef boost::shared_ptr<ScopeFactory> SharedScopeFactory;

}

}

#endif // RASK_AST_SCOPE_HPP
