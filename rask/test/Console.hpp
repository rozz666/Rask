// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
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