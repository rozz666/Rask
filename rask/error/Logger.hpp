// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_ERROR_LOGGER_HPP
#define RASK_ERROR_LOGGER_HPP

#include <vector>
#include <rask/Position.hpp>
#include <rask/error/Message.hpp>

namespace rask
{
namespace error
{

class Logger
{
public:

    typedef std::vector<Message> Errors;

    const Errors& errors() const
    {
        return errors_;
    }

    void log(const Message& msg)
    {
        errors_.push_back(msg);
    }

private:

    Errors errors_;
};

}
}

#endif /* RASK_ERROR_LOGGER_HPP */