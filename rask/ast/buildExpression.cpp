// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <rask/ast/Builder.hpp>

namespace rask
{
namespace ast
{

struct BuildExpression : boost::static_visitor<boost::optional<Expression> >
{
    Builder& builder;
    error::SharedLogger logger;
    SharedScope scope;

    BuildExpression(Builder& builder, SharedScope scope, error::SharedLogger logger)
        : builder(builder), logger(logger), scope(scope) { }

    Expression operator()(const cst::Constant& c)
    {
        return Constant(c.value);
    }

    boost::optional<Expression> operator()(const cst::Identifier& id)
    {
        if (id.value == "true") return ast::Expression(ast::Constant(true));
        if (id.value == "false") return ast::Expression(ast::Constant(false));

        boost::optional<SharedVariable> var = scope->getVariable(id.value);

        if (!var)
        {
            logger->log(error::Message::unknownIdentifier(id.position, id.value));
            return boost::none;
        }

        return Expression(*var);
    }

    boost::optional<Expression> operator()(const cst::FunctionCall& fc)
    {
        boost::optional<FunctionCall> c = builder.buildFunctionCall(fc, scope);

        if (!c) return boost::none;

        return ast::Expression(*c);
    }

    boost::optional<Expression> operator()(const cst::UnaryOperatorCall& oc)
    {
        boost::optional<FunctionCall> c = builder.buildUnaryOperatorCall(oc, scope);

        if (!c) return boost::none;

        return ast::Expression(*c);
    }

    boost::optional<Expression> operator()(const cst::ChainExpression& ce)
    {
        return builder.buildChainExpression(ce, scope);
    }
};

boost::optional<Expression> Builder::buildExpression(const cst::Expression& expr, SharedScope scope)
{
    BuildExpression b(*this, scope, logger_);
    return expr.apply_visitor(b);
}

}
}
