#ifndef RASK_CST_IDENTIFIER_HPP
#define RASK_CST_IDENTIFIER_HPP

#include <string>
#include <boost/fusion/adapted/struct/adapt_struct.hpp> 
#include <rask/Position.hpp>

namespace rask
{
namespace cst
{

struct Identifier
{
    Position position;
    std::string value;
};

}
}

BOOST_FUSION_ADAPT_STRUCT(
    rask::cst::Identifier,
    (rask::Position, position)
    (std::string, value)
)

#endif /* RASK_CST_IDENTIFIER_HPP */