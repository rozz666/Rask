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
    
typedef boost::variant<Constant, Identifier, boost::recursive_wrapper<FunctionCall> > UnaryExpression;

struct FunctionCall
{
    Identifier function;
    std::vector<UnaryExpression> args;
};

inline Constant& getConstant(UnaryExpression& e) { return boost::get<Constant>(e); }
inline const Constant& getConstant(const UnaryExpression& e) { return boost::get<Constant>(e); }

inline Identifier& getIdentifier(UnaryExpression& e) { return boost::get<Identifier>(e); }
inline const Identifier& getIdentifier(const UnaryExpression& e) { return boost::get<Identifier>(e); }

inline FunctionCall& getFunctionCall(UnaryExpression& e) { return boost::get<FunctionCall>(e); }
inline const FunctionCall& getFunctionCall(const UnaryExpression& e) { return boost::get<FunctionCall>(e); }

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::FunctionCall,
    (rask::cst::Identifier, function)
    (std::vector<rask::cst::UnaryExpression>, args)
)

#endif // RASK_CST_EXPRESSION_HPP
