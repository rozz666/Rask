// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_GTESTPRINTER_HPP
#define RASK_TEST_GTESTPRINTER_HPP

#include <iostream>
#include <boost/algorithm/string/replace.hpp>
#include <gtest/gtest.h>

namespace rask
{
namespace test
{

class GTestPrinter : public testing::EmptyTestEventListener
{
public:

    virtual void OnTestProgramStart(const testing::UnitTest& test);
    virtual void OnTestProgramEnd(const testing::UnitTest& test);
    virtual void OnTestCaseStart(const testing::TestCase& testCase);
    virtual void OnTestCaseEnd(const testing::TestCase&);
    virtual void OnTestEnd(const testing::TestInfo& testInfo);
};

}
}

#endif // RASK_TEST_GTESTPRINTER_HPP
