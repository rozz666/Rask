// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cst/Grammar.hpp>
#include <rask/cst/parseFile.hpp>

namespace rask
{
namespace cst
{

void optimizeChainExpression(ChainExpression& ce);

struct ExpressionOptimizer : boost::static_visitor<Expression>
{
    Expression operator()(const Constant& c) { return c; }

    Expression operator()(const Identifier& id) { return id; }

    Expression operator()(const FunctionCall& fc)
    {
        FunctionCall r = fc;

        BOOST_FOREACH(ChainExpression& ce, r.args)
        {
             optimizeChainExpression(ce);
        }
        return r;
    }

    Expression operator()(const UnaryOperatorCall& uoc)
    {
        UnaryOperatorCall r;
        r.op = uoc.op;
        r.expr = uoc.expr.apply_visitor(*this);
        return r;
    }

    Expression operator()(const ChainExpression& ce)
    {
        if (ce.next.empty()) return ce.expr.apply_visitor(*this);
        ChainExpression r;
        r.expr = ce.expr.apply_visitor(*this);
        r.next.reserve(ce.next.size());
        BOOST_FOREACH(const OpExpr& oe, ce.next)
        {
            OpExpr roe;
            roe.op = oe.op;
            roe.expr = oe.expr.apply_visitor(*this);
            r.next.push_back(roe);
        }
        return r;
    }
};

Expression optimizeExpression(const Expression& expr)
{
    ExpressionOptimizer eo;
    return expr.apply_visitor(eo);
}

void optimizeChainExpression(ChainExpression& ce)
{
    Expression e = optimizeExpression(ce);

    if (e.type() == typeid(ChainExpression))
    {
        ce = getChainExpression(e);
    }
    else
    {
        ce.expr = e;
        ce.next.clear();
    }
}

struct Optimizer : boost::static_visitor<>
{
    void operator()(VariableDecl& vd)
    {
        if (vd.value) optimizeChainExpression(*vd.value);
    }

    void operator()(Return& ret)
    {
        optimizeChainExpression(ret.value);
    }

    void operator()(FunctionCall& fc)
    {
        BOOST_FOREACH(ChainExpression& ce, fc.args)
        {
            optimizeChainExpression(ce);
        }
    }
};

void optimize(Tree& tree)
{
    Optimizer optimizer;
    BOOST_FOREACH(Function& f, tree.functions)
    {
        BOOST_FOREACH(Statement& s, f.stmts)
        {
            s.apply_visitor(optimizer);
        }
    }
}

boost::optional<Tree> parseFile(InputStream& is, error::Logger& logger)
{
    typedef std::vector<char> Source;
    Source source;

    source.assign(std::istreambuf_iterator<char>(is.stream()), std::istreambuf_iterator<char>());

    typedef boost::spirit::classic::position_iterator<Source::const_iterator, boost::spirit::classic::file_position> PosIterator;

    using boost::spirit::ascii::space;
    PosIterator iter(source.begin(), source.end(), is.file());
    PosIterator end;

    typedef rask::cst::Grammar<PosIterator> Grammar;
    Grammar grammar(logger);
    cst::Tree cst;

    bool r = boost::spirit::qi::phrase_parse(iter, end, grammar, space, cst);

    if (r && iter == end && logger.errors().empty() && cst.functions.size() > 0)
    {
        optimize(cst);
        return cst;
    }
    else
    {
        if (iter == end && logger.errors().empty())
        {
            logger.log(error::Message::missingMainFunction(Position(is.file())));
        }

        return boost::none;
    }
}

}
}