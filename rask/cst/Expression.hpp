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
    
typedef boost::variant<
    Constant,
    Identifier,
    boost::recursive_wrapper<FunctionCall>,
    boost::recursive_wrapper<UnaryOperatorCall>
> UnaryExpression;

struct BinaryOperator
{
    enum Tag { PLUS, MINUS };
    Position position;
    Tag tag;
};

template <typename ExpressionType>
struct OpExpr
{
    BinaryOperator op;
    ExpressionType expr;
};

struct Expression
{
    UnaryExpression first;
    std::vector<OpExpr<UnaryExpression> > next;
};

struct FunctionCall
{
    Identifier function;
    std::vector<UnaryExpression> args;
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
    UnaryExpression expr;
};

inline Constant& getConstant(UnaryExpression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const UnaryExpression& e) { return boost::get<Constant>(e); }

inline Identifier& getIdentifier(UnaryExpression& e) { return boost::get<Identifier>(e); }
inline const Identifier& getIdentifier(const UnaryExpression& e) { return boost::get<Identifier>(e); }

inline FunctionCall& getFunctionCall(UnaryExpression& e) { return boost::get<FunctionCall>(e); }
inline const FunctionCall& getFunctionCall(const UnaryExpression& e) { return boost::get<FunctionCall>(e); }

inline UnaryOperatorCall& getUnaryOperatorCall(UnaryExpression& e) { return boost::get<UnaryOperatorCall>(e); }
inline const UnaryOperatorCall& getUnaryOperatorCall(const UnaryExpression& e) { return boost::get<UnaryOperatorCall>(e); }

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::BinaryOperator,
    (rask::Position, position)
    (rask::cst::BinaryOperator::Tag, tag)
)

BOOST_FUSION_ADAPT_TPL_STRUCT(
    (ExpressionType),
    (rask::cst::OpExpr) (ExpressionType),
    (rask::cst::BinaryOperator, op)
    (ExpressionType, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Expression,
    (rask::cst::UnaryExpression, first)
    (std::vector<rask::cst::OpExpr<rask::cst::UnaryExpression> >, next)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::FunctionCall,
    (rask::cst::Identifier, function)
    (std::vector<rask::cst::UnaryExpression>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::UnaryOperator,
    (rask::Position, position)
    (rask::cst::UnaryOperator::Tag, tag)
)

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::UnaryOperatorCall,
    (rask::cst::UnaryOperator, op)
    (rask::cst::UnaryExpression, expr)
)

#endif // RASK_CST_EXPRESSION_HPP
