/*
// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
*/
#ifndef RASK_AST_SYMBOLTABLE_HPP
#define RASK_AST_SYMBOLTABLE_HPP

#include <map>
#include <stdexcept>
#include <rask/cst/Identifier.hpp>
#include <rask/ast/Variable.hpp>
#include <rask/ast/Function.hpp>

namespace rask
{
namespace ast
{

class SymbolTableError : public std::logic_error
{
public:

    SymbolTableError(const std::string& msg) : std::logic_error(msg) { }
};
   
    
class SymbolTable
{
public:
    
    SharedVariable add(SharedVariable var)
    {
        return add(vars_, var);
    }

    SharedFunction add(SharedFunction function)
    {
        return add(functions_, function);
    }
    
    SharedVariable getVariable(const std::string& name) const
    {
        return get(vars_, name);
    }

    SharedFunction getFunction(const std::string& name) const
    {
        return get(functions_, name);
    }

private:

    typedef std::map<std::string, SharedVariable> Vars;
    typedef std::map<std::string, SharedFunction> Functions;

    Vars vars_;
    Functions functions_;

    template <typename M>
    static typename M::mapped_type add(M& m, const typename M::mapped_type& v)
    {
        return m.insert(typename M::value_type(v->name().value, v)).first->second;
    }

    template <typename M>
    static typename M::mapped_type get(const M& m, const std::string& name)
    {
        typename M::const_iterator it = m.find(name);
        
        if (it == m.end()) throw SymbolTableError("Symbol \'" + name + "\' not found");
        
        return it->second;
    }

};

}
}

#endif // RASK_AST_SYMBOLTABLE_HPP
