// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <rask/ast/Builder.hpp>
#include <rask/ast/operatorName.hpp>

namespace rask
{
namespace ast
{

struct BuildExpression : boost::static_visitor<boost::optional<Expression> >
{
    Builder& builder;
    error::Logger& logger;
    const SymbolTable& st;

    BuildExpression(Builder& builder, const SymbolTable& st, error::Logger& logger)
        : builder(builder), logger(logger), st(st) { }

    Expression operator()(const cst::Constant& c)
    {
        return Constant(c.value);
    }

    boost::optional<Expression> operator()(const cst::Identifier& id)
    {
        boost::optional<SharedVariable> var = st.getVariable(id.value);

        if (!var)
        {
            logger.log(error::Message::unknownIdentifier(id.position, id.value));
            return boost::none;
        }
        
        return Expression(*var);
    }

    boost::optional<Expression> operator()(const cst::FunctionCall& fc)
    {
        boost::optional<FunctionCall> c = builder.buildFunctionCall(fc);

        if (!c) return boost::none;

        return ast::Expression(*c);
    }

    boost::optional<Expression> operator()(const cst::UnaryOperatorCall& oc)
    {
        boost::optional<FunctionCall> c = builder.buildUnaryOperatorCall(oc);

        if (!c) return boost::none;
        
        return ast::Expression(*c);
    }

    boost::optional<Expression> operator()(const cst::ChainExpression& ce)
    {
        throw std::runtime_error(
            "boost::optional<Expression> operator()(const cst::ChainExpression& ce) not implemented");
    }
};
    
boost::optional<Expression> Builder::buildUnaryExpression(const cst::Expression& expr)
{
    BuildExpression b(*this, symbolTable_, logger_);
    return expr.apply_visitor(b);
}

boost::optional<Expression> Builder::buildChainExpression(const cst::ChainExpression& expr)
{
    boost::optional<Expression> left = buildUnaryExpression(expr.expr);

    if (!left) return boost::none;
    
    for(std::size_t i = 0; i != expr.next.size(); ++i)
    {
        boost::optional<Expression> right = buildUnaryExpression(expr.next[i].expr);

        if (!right) return boost::none;

        FunctionCall::Arguments args(2);
        args[0] = *left;
        args[1] = *right;

        left = FunctionCall(*symbolTable_.getFunction(operatorName(expr.next[i].op.tag)), args);
    }

    return left;
}


}
}
