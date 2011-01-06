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
#include <boost/optional.hpp>
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

    SharedFunction add(SharedFunction function)
    {
        return functions_.insert(Functions::value_type(function->name().value, function)).first->second;
    }

    boost::optional<SharedFunction> getFunction(const std::string& name) const
    {
        typename Functions::const_iterator it = functions_.find(name);
        
        if (it == functions_.end()) return boost::none;
        
        return it->second;
    }

private:

    typedef std::map<std::string, SharedFunction> Functions;

    Functions functions_;
};

}
}

#endif // RASK_AST_SYMBOLTABLE_HPP
