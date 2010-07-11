#include <rask/Position.hpp>

namespace rask
{

std::ostream& operator<<(std::ostream& os, const Position& p)
{
    std::ostream::sentry ok(os);

    if (ok)
    {
        auto flags = os.flags();

        os << std::dec << p.file << '(' << p.row << ", " << p.column << ")";

        os.flags(flags);
    }

    return os;
} 

}