// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_EXPRESSION_HPP
#define RASK_CST_EXPRESSION_HPP

#include <vector>
#include <boost/variant.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <rask/cst/Constant.hpp>
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cst
{

struct FunctionCall;
struct UnaryOperatorCall;
struct ChainExpression;

typedef boost::variant<
    Constant,
    Identifier,
    boost::recursive_wrapper<FunctionCall>,
    boost::recursive_wrapper<UnaryOperatorCall>,
    boost::recursive_wrapper<ChainExpression>
> Expression;

struct BinaryOperator
{
    enum Tag { PLUS, MINUS, MULT, DIV, MOD };
    Position position;
    Tag tag;
};

struct OpExpr
{
    BinaryOperator op;
    Expression expr;
};

struct ChainExpression
{
    Expression expr;
    std::vector<OpExpr> next;
};

struct FunctionCall
{
    Identifier function;
    std::vector<Expression> args;
};

struct UnaryOperator
{
    enum Tag { MINUS };
    Position position;
    Tag tag;
};

struct UnaryOperatorCall
{
    UnaryOperator op;
    Expression expr;
};

inline Constant& getConstant(Expression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const Expression& e) { return boost::get<Constant>(e); }

inline Identifier& getIdentifier(Expression& e) { return boost::get<Identifier>(e); }
inline const Identifier& getIdentifier(const Expression& e) { return boost::get<Identifier>(e); }

inline FunctionCall& getFunctionCall(Expression& e) { return boost::get<FunctionCall>(e); }
inline const FunctionCall& getFunctionCall(const Expression& e) { return boost::get<FunctionCall>(e); }

inline UnaryOperatorCall& getUnaryOperatorCall(Expression& e) { return boost::get<UnaryOperatorCall>(e); }
inline const UnaryOperatorCall& getUnaryOperatorCall(const Expression& e) { return boost::get<UnaryOperatorCall>(e); }

inline ChainExpression& getChainExpression(Expression& e) { return boost::get<ChainExpression>(e); }
inline const ChainExpression& getChainExpression(const Expression& e) { return boost::get<ChainExpression>(e); }

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::BinaryOperator,
    (rask::Position, position)
    (rask::cst::BinaryOperator::Tag, tag)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::OpExpr,
    (rask::cst::BinaryOperator, op)
    (rask::cst::Expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::ChainExpression,
    (rask::cst::Expression, expr)
    (std::vector<rask::cst::OpExpr>, next)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::FunctionCall,
    (rask::cst::Identifier, function)
    (std::vector<rask::cst::Expression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::UnaryOperator,
    (rask::Position, position)
    (rask::cst::UnaryOperator::Tag, tag)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::UnaryOperatorCall,
    (rask::cst::UnaryOperator, op)
    (rask::cst::Expression, expr)
)

#endif // RASK_CST_EXPRESSION_HPP
