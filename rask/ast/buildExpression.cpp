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
    error::Logger& logger;
    const SymbolTable& st;

    BuildExpression(const SymbolTable& st, error::Logger& logger)
        : logger(logger), st(st) { }

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
};
    
boost::optional<Expression> Builder::buildExpression(const cst::Expression& expr, const SymbolTable& st)
{
    BuildExpression b(st, logger_);
    return expr.apply_visitor(b);
}

}
}


