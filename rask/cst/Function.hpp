#ifndef RASK_CST_FUNCTION_HPP
#define RASK_CST_FUNCTION_HPP

#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/cst/Identifier.hpp>

namespace rask
{
namespace cst
{

struct Function
{
    Identifier name;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Function,
    (rask::cst::Identifier, name)
)

#endif /* RASK_CST_FUNCTION_HPP */