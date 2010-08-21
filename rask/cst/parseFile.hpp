// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CST_PARSEMAIN_HPP
#define RASK_CST_PARSEMAIN_HPP

#include <boost/optional.hpp>
#include <rask/InputStream.hpp>
#include <rask/error/Logger.hpp>
#include <rask/cst/Function.hpp>

namespace rask
{
namespace cst
{

boost::optional<Function> parseFile(InputStream& is, error::Logger& logger);

}

}

#endif /* RASK_CST_PARSEMAIN_HPP */