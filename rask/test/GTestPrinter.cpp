// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/GTestPrinter.hpp>

namespace rask
{
namespace test
{

void GTestPrinter::OnTestProgramStart(const testing::UnitTest& test)
{
    std::cout << std::endl;
}

void GTestPrinter::OnTestProgramEnd(const testing::UnitTest& test)
{
    for (int i = 0; i < test.total_test_case_count(); ++i)
    {
        const testing::TestCase& testCase = *test.GetTestCase(i);

        for (int j = 0; j < testCase.total_test_count(); ++j)
        {
            const testing::TestInfo& testInfo = *testCase.GetTestInfo(j);

            for (int k = 0; k != testInfo.result()->total_part_count(); ++k)
            {
                const testing::TestPartResult& result = testInfo.result()->GetTestPartResult(k);
                std::cout << "\n" << testInfo.name() << ":\n";

                if (result.file_name())
                {
                    std::cout << result.file_name() << ":" << result.line_number() << std::endl;
                }

                std::cout << result.summary() << std::endl;
            }
        }
    }

    std::cout << std::endl << "tests summary:";

    std::cout << " ok:" << test.successful_test_count();

    if (test.successful_test_count() < test.total_test_count())
        std::cout << " failed:" << (test.total_test_count() - test.successful_test_count());

    std::cout << std::endl;
}

void GTestPrinter::OnTestCaseStart(const testing::TestCase& testCase)
{
    std::string name = testCase.name();
    boost::replace_all(name, "_", ".");
    std::cout << name << ":" << std::flush;
}

void GTestPrinter::OnTestCaseEnd(const testing::TestCase&)
{
    std::cout << std::endl;
}

void GTestPrinter::OnTestEnd(const testing::TestInfo& testInfo)
{
    std::cout << (testInfo.result()->Passed() ? "." : "F");
}

}
}
