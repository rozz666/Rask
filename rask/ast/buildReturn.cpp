// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/ast/Builder.hpp>

namespace rask
{

namespace ast
{

boost::optional<Return> Builder::buildReturn(const cst::Return& ret)
{
    return boost::none;
}

}
}