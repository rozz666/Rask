// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <tut/tut_restartable.hpp>
#include <tut/tut.hpp>
#include <rask/ut/tut_color_console_reporter.hpp>

namespace tut
{
    
test_runner_singleton runner;

}

int main()
{

    try
    {
        tut::color_console_reporter reporter;
        tut::restartable_wrapper restartable;
        restartable.set_callback(&reporter);
        restartable.run_tests();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "tut raised ex: " << ex.what() << std::endl;
        return 1;
    }
} 