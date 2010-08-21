// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
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
            os << ':' << p.row;
            
            if (p.column > 0)
            {
                os << ':' << p.column;
            }
        }

        os.flags(flags);
    }

    return os;
} 

}