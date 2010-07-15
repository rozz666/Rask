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

    const Errors errors() const
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