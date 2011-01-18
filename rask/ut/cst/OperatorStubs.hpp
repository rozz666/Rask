#ifndef RASK_CST_OPERATORSTUBS_HPP
#define RASK_CST_OPERATORSTUBS_HPP

#include <ostream>
#include <rask/cst/Expression.hpp>

namespace rask
{
namespace cst
{

inline std::ostream& operator<<(std::ostream& os, const Expression& )
{
    return os << "rask::cst::Expression";
}

}
}

#endif /* RASK_CST_OPERATORSTUBS_HPP */