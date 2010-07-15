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