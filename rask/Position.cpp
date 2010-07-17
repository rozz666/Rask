#include <rask/Position.hpp>

namespace rask
{

std::ostream& operator<<(std::ostream& os, const Position& p)
{
    std::ostream::sentry ok(os);

    if (ok)
    {
        auto flags = os.flags();

        os << std::dec << p.file;
        
        if (p.row > 0)
        {
            os << '(' << p.row;
            
            if (p.column > 0)
            {
                os << ", " << p.column;
            }

            os << ')';
        }

        os.flags(flags);
    }

    return os;
} 

}