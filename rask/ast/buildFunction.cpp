// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <sstream>
#include <boost/foreach.hpp>
#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

struct StatementVisitor : boost::static_visitor<bool>
{
    Builder& b;
    Function& f;

    StatementVisitor(Builder& b, Function& f) : b(b), f(f) { }

    bool operator()(const cst::FunctionCall& call)
    {
        boost::optional<ast::FunctionCall> fc = b.buildFunctionCall(call);

        if (!fc) return false;

        f.addStmt(*fc);

        return true;
    }

    bool operator()(const cst::VarDecl& vd)
    {
        SharedVarDecl d = b.buildVarDecl(vd);
        
        if (!d) return false;
        
        f.addStmt(d);

        return true;
    }
};
    
boost::optional<FunctionCall> Builder::buildFunctionCall(const cst::FunctionCall& call)
{
    if (call.function.value != "print")
    {
        logger_.log(error::Message::unknownIdentifier(call.function.position, call.function.value));
        return boost::none;
    }
    
    if (call.args.size() != 1)
    {
        logger_.log(error::Message::functionNotFound(call.function.position, functionSignature("print", call.args)));
        return boost::none;
    }
    
    return boost::get<cst::Constant>(call.args[0]).value;
}

boost::optional<Function> Builder::buildFunction(const cst::Function& cf)
{
    Function af;

    BOOST_FOREACH(const cst::Statement& stmt, cf.stmts)
    {
        StatementVisitor sv(*this, af);
        if (!stmt.apply_visitor(sv)) return boost::none;
    }

    return af;
}

std::string Builder::functionSignature(const std::string& name, const std::vector<cst::Expression>& args)
{
    std::ostringstream os;
    
    os << name << "(";
    
    if (!args.empty())
    {
        os << "int";

        std::fill_n(std::ostream_iterator<std::string>(os), args.size() - 1, ", int");
    }
   
    os << ")";
    return os.str();
}

}
}