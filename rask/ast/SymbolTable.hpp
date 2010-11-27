// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_AST_SYMBOLTABLE_HPP
#define RASK_AST_SYMBOLTABLE_HPP

#include <map>
#include <stdexcept>
#include <rask/cst/Identifier.hpp>
#include <rask/ast/Variable.hpp>
#include <rask/ast/CustomFunction.hpp>

namespace rask
{
namespace ast
{
    
class SymbolTable
{
public:

    SymbolTable() : vars_(1) { }


    SharedVariable add(SharedVariable var)
    {
        return add(vars_.back(), var);
    }

    SharedFunction add(SharedFunction function)
    {
        return add(functions_, function);
    }

    boost::optional<SharedVariable> getVariable(const std::string& name) const
    {
        return get(vars_.back(), name);
    }

    boost::optional<SharedFunction> getFunction(const std::string& name) const
    {
        return get(functions_, name);
    }

    void enterScope()
    {
        vars_.push_back(Vars::value_type());
    }

    void exitScope()
    {
        if (vars_.size() == 1) throw std::out_of_range("SymbolTable exit scope");

        vars_.pop_back();
    }

private:

    typedef std::vector<std::map<std::string, SharedVariable> > Vars;
    typedef std::map<std::string, SharedFunction> Functions;

    Vars vars_;
    Functions functions_;

    template <typename M>
    static typename M::mapped_type add(M& m, const typename M::mapped_type& v)
    {
        return m.insert(typename M::value_type(v->name().value, v)).first->second;
    }

    template <typename M>
    static boost::optional<typename M::mapped_type> get(const M& m, const std::string& name)
    {
        typename M::const_iterator it = m.find(name);
        
        if (it == m.end()) return boost::none;
        
        return it->second;
    }

};

}
}

#endif // RASK_AST_SYMBOLTABLE_HPP
