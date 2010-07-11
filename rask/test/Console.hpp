#ifndef RASK_TEST_CONSOLE_HPP
#define RASK_TEST_CONSOLE_HPP

#include <ostream>

namespace rask
{
namespace test
{

std::ostream& rc(std::ostream& os);
std::ostream& green(std::ostream& os);
std::ostream& red(std::ostream& os);

}
}

#endif /* RASK_TEST_CONSOLE_HPP */