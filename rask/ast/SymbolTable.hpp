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
    typedef std::map<std::string, SharedVariable> Vars;
    
public:
    
    void add(SharedVariable var)
    {
        vars_.insert(Vars::value_type(var->name().value, var));
    }
    
    SharedVariable get(const cst::Identifier& name) const
    {
        Vars::const_iterator it = vars_.find(name.value);

        if (it == vars_.end()) throw SymbolTableError("Symbol \'" + name.value + "\' not found");
        
        return it->second;
    }

private:

    Vars vars_;
};

}
}

#endif // RASK_AST_SYMBOLTABLE_HPP
