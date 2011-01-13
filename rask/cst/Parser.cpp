// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cst/Parser.hpp>
#include <rask/cst/parseFile.hpp>

namespace rask
{
namespace cst
{

boost::optional<Tree> Parser::parseFile(InputStream& is)
{
    return rask::cst::parseFile(is, logger_);
}

}
}
