// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_SYMBOLTABLE_HPP
#define RASK_CG_SYMBOLTABLE_HPP

#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <llvm/Instructions.h>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cg
{

class SymbolTableError : public std::logic_error
{
public:

    SymbolTableError(const std::string& msg) : std::logic_error(msg) { }
};

class SymbolTable
{
    typedef std::map<std::pair<std::string, Position>, llvm::AllocaInst *> Values;

public:

    void add(const cst::Identifier& id, llvm::AllocaInst *value)
    {
        if (!values_.insert(Values::value_type(std::make_pair(id.value, id.position), value)).second)
        {
            throw SymbolTableError("Duplicated identifier");
        }
    }

    llvm::AllocaInst *get(const cst::Identifier& id) const
    {
        Values::const_iterator it = values_.find(std::make_pair(id.value, id.position));

        if (it == values_.end()) throw SymbolTableError("Symbol \'" + id.value + "\' not found");

        return it->second;
    }

private:

    Values values_;
};

typedef boost::shared_ptr<SymbolTable> SharedSymbolTable;

}
}

#endif // RASK_CG_SYMBOLTABLE_HPP
