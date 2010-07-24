#ifndef RASK_CST_FUNCTION_HPP
#define RASK_CST_FUNCTION_HPP

#include <vector>
#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Identifier.hpp>
#include <rask/cst/FunctionCall.hpp>

namespace rask
{
namespace cst
{

struct Function
{
    Identifier name;
    std::vector<FunctionCall> calls;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Function,
    (rask::cst::Identifier, name)
    (std::vector<rask::cst::FunctionCall>, calls)
)

#endif /* RASK_CST_FUNCTION_HPP */