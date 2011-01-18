// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <map>
#include <boost/foreach.hpp>
#include <rask/ast/Builder.hpp>
#include <boost/assign/list_of.hpp>

namespace rask
{
namespace ast
{

typedef std::map<std::string, BasicType> TypeDictionary;
const TypeDictionary typeDictionary = boost::assign::map_list_of
    ("int32", INT32)
    ("boolean", BOOLEAN)
    ("void", VOID);

boost::optional<FunctionDecl> Builder::buildFunctionDecl(const cst::Function& f)
{
    FunctionDecl fd(f.name, typeDictionary.find(f.type.value)->second);
    SharedCustomFunction cf = fd.function();

    SharedFunction r = functionTable_->add(cf);

    if (r != cf)
    {
        logger_->log(error::Message::redefinition(f.name.position, f.name.value + "()"));
        logger_->log(error::Message::previousDefinition(r->name().position, r->name().value + "()"));
        return boost::none;
    }

    BOOST_FOREACH(const cst::FunctionArgument& arg, f.args)
    {
        cf->addArg(variableFactory_->createVariable(arg.name, typeDictionary.find(arg.type.value)->second));
    }

    return fd;
}

}
}