#ifndef RASK_ERROR_LOGGER_HPP
#define RASK_ERROR_LOGGER_HPP

#include <vector>

namespace rask
{
namespace error
{

class Logger
{
public:

    typedef std::vector<int> Errors;

    const Errors errors() const
    {
        return Errors();
    }
};

}
}

#endif /* RASK_ERROR_LOGGER_HPP */