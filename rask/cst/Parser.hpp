// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_PARSER_HPP
#define RASK_CST_PARSER_HPP

#include <boost/optional.hpp>
#include <rask/InputStream.hpp>
#include <rask/error/Logger.hpp>
#include <rask/cst/Tree.hpp>
#include <di/constructor.hpp>

namespace rask
{
namespace cst
{

class Parser
{
public:

    DI_CONSTRUCTOR(Parser, (error::Logger& logger)) : logger_(logger) { }
    boost::optional<Tree> parseFile(InputStream& is);

private:

    error::Logger& logger_;
};

}

}

#endif // RASK_CST_PARSER_HPP
