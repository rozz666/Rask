// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/error/Message.hpp>

namespace rask
{
namespace error
{

std::ostream& operator<<(std::ostream& os, const Message& m)
{
    return os << m.position() << ": error: " << m.text();
}

}
}