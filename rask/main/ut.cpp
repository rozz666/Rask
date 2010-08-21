// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <iostream>
#include <rask/ut/tut_color_console_reporter.hpp>

namespace tut
{
    
test_runner_singleton runner;

}

int main()
{
    tut::color_console_reporter reporter;
    tut::runner.get().set_callback(&reporter);

    try
    {
        tut::runner.get().run_tests();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "tut raised ex: " << ex.what() << std::endl;
        return 1;
    }
} 