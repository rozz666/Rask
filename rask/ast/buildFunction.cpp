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
    CustomFunction& f;
    SharedScope scope;

    StatementVisitor(Builder& b, CustomFunction& f, SharedScope scope)
        : b(b), f(f), scope(scope) { }

    bool operator()(const cst::FunctionCall& call)
    {
        boost::optional<ast::FunctionCall> fc = b.buildFunctionCall(call, scope);

        if (!fc) return false;

        f.addStmt(*fc);

        return true;
    }

    bool operator()(const cst::VariableDecl& vd)
    {
        boost::optional<ast::VariableDecl> d = b.buildVariableDecl(vd, scope);
        
        if (!d) return false;
        
        f.addStmt(*d);

        return true;
    }

    bool operator()(const cst::Return& ret)
    {
        boost::optional<ast::Return> r = b.buildReturn(ret, scope);

        if (!r) return false;

        f.addStmt(*r);
        
        return true;
    }
};

bool Builder::buildFunction(const cst::Function& cf, SharedScope scope)
{
    SharedCustomFunction f = boost::dynamic_pointer_cast<CustomFunction>(*symbolTable_.getFunction(cf.name.value));

    for (unsigned short i = 0; i != f->argCount(); ++i)
    {
        scope->addVariable(f->arg(i));
    }

    BOOST_FOREACH(const cst::Statement& stmt, cf.stmts)
    {
        StatementVisitor sv(*this, *f, scope);
        if (!stmt.apply_visitor(sv)) return false;
    }

    return true;
}

std::string Builder::functionSignature(const std::string& name, const std::vector<cst::Expression>& args)
{
    std::ostringstream os;
    
    os << name << "(";
    
    if (!args.empty())
    {
        os << "int32";

        std::fill_n(std::ostream_iterator<std::string>(os), args.size() - 1, ", int32");
    }
   
    os << ")";
    return os.str();
}

}
}