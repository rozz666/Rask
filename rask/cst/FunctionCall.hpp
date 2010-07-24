#ifndef RASK_CST_FUNCTIONCALL_HPP
#define RASK_CST_FUNCTIONCALL_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cst
{

struct FunctionCall
{
    Identifier function;
    std::vector<int> args;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::FunctionCall,
    (rask::cst::Identifier, function)
    (std::vector<int>, args)
)

#endif /* RASK_CST_FUNCTIONCALL_HPP */