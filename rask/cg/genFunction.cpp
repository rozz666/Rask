// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/genFunction.hpp>

namespace rask
{
namespace cg
{

BytecodeBuffer genFunction(const ast::Function& f)
{
    BytecodeBuffer bb;
    bb.reserve(f.valueCount());
    
    for (std::size_t i = 0; i != f.valueCount(); ++i)
    {
        bb.push_back(f.value(i));
    }

    return bb;
}

}
}
