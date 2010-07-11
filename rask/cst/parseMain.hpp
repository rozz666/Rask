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

boost::optional<Function> parseMain(InputStream& is, error::Logger& logger);

}

}

#endif /* RASK_CST_PARSEMAIN_HPP */