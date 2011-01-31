// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/test/GTestController.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <cstdio>
#include <boost/shared_ptr.hpp>

namespace {
std::string journalName = "journal.gt";

void safefclose(FILE *f)
{
    if (f) std::fclose(f);
}

}

namespace rask
{
namespace test
{

GTestController::GTestController() : ok_(0), failed_(0), crashed_(0)
{
    loadState();
    testing::GTEST_FLAG(filter) = filter_.str();
}

void GTestController::OnTestProgramStart(const testing::UnitTest& )
{
    if (currentSuite_.empty()) print("\n").flush();
}

void GTestController::OnTestCaseStart(const testing::TestCase& testCase)
{
    if (testCase.name() != currentSuite_)
    {
        currentSuite_ = testCase.name();
        print("\n").print(prettyName(testCase.name())).print(":").flush();
    }
}

void GTestController::OnTestStart(const testing::TestInfo& testInfo)
{
    filter_ << testInfo.test_case_name() << "." << testInfo.name() << ":";
    saveState(testInfo.test_case_name(), testInfo.name());
}

void GTestController::OnTestEnd(const testing::TestInfo& testInfo)
{
    if (testInfo.result()->Passed())
    {
        ++ok_;
        print(".");
    }
    else
    {
        ++failed_;
        print("F");
    }
    flush();

    for (int k = 0; k != testInfo.result()->total_part_count(); ++k)
    {
        const testing::TestPartResult& result = testInfo.result()->GetTestPartResult(k);
        errors_ << "\n" << testInfo.name() << ":\n";

        if (result.file_name())
        {
            errors_ << result.file_name() << ":" << result.line_number() << std::endl;
        }

        errors_ << result.summary() << std::endl;
    }
}

void GTestController::OnTestProgramEnd(const testing::UnitTest& )
{
    std::remove(journalName.c_str());
    std::cout << errors_.str() << std::endl;

    std::cout << "tests summary:";

    if (ok_) std::cout << " ok:" << ok_;
    if (failed_) std::cout << " failed:" << failed_;
    if (crashed_) std::cout << " crashed:" << crashed_;

    std::cout << std::endl;
}

void GTestController::loadState()
{
    boost::shared_ptr<FILE> f(std::fopen(journalName.c_str(), "rb"), safefclose);

    if (!validFile(f.get()))
    {
        filter_ << "*:-:";
        return;
    }

    currentSuite_ = readString(f.get());
    std::string failedTest = readString(f.get());
    filter_ << readString(f.get());
    print(readString(f.get())).flush();
    errors_ << readString(f.get());
    std::fread(&ok_, sizeof(ok_), 1, f.get());
    std::fread(&failed_, sizeof(failed_), 1, f.get());
    std::fread(&crashed_, sizeof(crashed_), 1, f.get());

    ++crashed_;
    print("C").flush();
    errors_ << "\n" << prettyName(currentSuite_) << "." << failedTest << ":\nCRASHED!!!\n";
}

void GTestController::saveState(const std::string& currentSuite, const std::string currentTest)
{
    boost::shared_ptr<FILE> f(std::fopen(journalName.c_str(), "wb"), safefclose);

    if (!f) return;
    char good = 'N';
    std::fwrite(&good, sizeof(good), 1, f.get());
    writeString(currentSuite, f.get());
    writeString(currentTest, f.get());
    writeString(filter_.str(), f.get());
    writeString(output_.str(), f.get());
    writeString(errors_.str(), f.get());
    std::fwrite(&ok_, sizeof(ok_), 1, f.get());
    std::fwrite(&failed_, sizeof(failed_), 1, f.get());
    std::fwrite(&crashed_, sizeof(crashed_), 1, f.get());
    std::fseek(f.get(), 0, SEEK_SET);
    good = 'Y';
    std::fwrite(&good, sizeof(good), 1, f.get());
}
void GTestController::writeString(const std::string& s, FILE* f)
{
    std::size_t n = s.length();
    std::fwrite(&n, sizeof(n), 1, f);
    std::fwrite(s.c_str(), 1, n, f);
}

std::string GTestController::readString(FILE* f)
{
    std::size_t n;
    std::fread(&n, sizeof(n), 1, f);
    std::vector<char> buf(n);
    std::fread(&buf[0], 1, n, f);
    return std::string(buf.begin(), buf.end());
}

bool GTestController::validFile(FILE* f)
{
    char good;
    return f && std::fread(&good, sizeof(good), 1, f) && (good == 'Y');
}
std::string GTestController::prettyName(std::string name)
{
    boost::replace_all(name, "_", ".");
    return name;
}


}
}