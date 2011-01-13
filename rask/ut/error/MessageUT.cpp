// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/error/Message.hpp>
#include <sstream>
#include <gtest/gtest.h>

using namespace rask;
using error::Message;

struct rask_error_Message : testing::Test
{
    Position pos;

    rask_error_Message() : pos("sample.rask", 1, 2) { }

    void assertMessage(const std::string& expected, const Message& msg)
    {
        ASSERT_EQ(Message(pos, expected), msg);
    }
};


TEST_F(rask_error_Message, messagePositionAndText)
{
    Position pos("abc", 123, 456);
    std::string text = "This is an error";
    Message msg(pos, text);

    ASSERT_EQ(pos, msg.position());
    ASSERT_EQ(text, msg.text());
}

TEST_F(rask_error_Message, equality)
{
    Message msg1(Position("asia", 1, 2), "error");
    Message msg2(Position("kasia", 1, 2), "error");
    Message msg3(Position("asia", 1, 2), "xxx");

    ASSERT_TRUE(msg1 == msg1);
    ASSERT_FALSE(msg1 != msg1);
    ASSERT_TRUE(msg1 != msg2);
    ASSERT_FALSE(msg1 == msg2);
    ASSERT_TRUE(msg1 != msg3);
    ASSERT_FALSE(msg1 == msg3);
}

TEST_F(rask_error_Message, shiftToStream)
{
    std::ostringstream ss1, ss2;
    std::string msg = "xxx";
    ss1 << Message(pos, msg);
    ss2 << pos << ": error: " << msg;
    ASSERT_EQ(ss2.str(), ss1.str());
}

TEST_F(rask_error_Message, missingMainFunction)
{
    assertMessage("missing main function", Message::missingMainFunction(pos));
}

TEST_F(rask_error_Message, missingReturnType)
{
    assertMessage("missing return type", Message::missingReturnType(pos));
}

TEST_F(rask_error_Message, missingOpeningBrace)
{
    assertMessage("missing opening brace \'{\'", Message::missingOpeningBrace(pos));
}

TEST_F(rask_error_Message, missingClosingBrace)
{
    assertMessage("missing closing brace \'}\'", Message::missingClosingBrace(pos));
}

TEST_F(rask_error_Message, missingOpeningParen)
{
    assertMessage("missing opening parenthesis \'(\'", Message::missingOpeningParen(pos));
}

TEST_F(rask_error_Message, missingClosingParen)
{
    assertMessage("missing closing parenthesis \')\'", Message::missingClosingParen(pos));
}

TEST_F(rask_error_Message, missingRightArrow)
{
    assertMessage("missing \'->\'", Message::missingRightArrow(pos));
}

TEST_F(rask_error_Message, unknownIdentifier)
{
    assertMessage("unknown identifier \'abcd\'", Message::unknownIdentifier(pos, "abcd"));
}

TEST_F(rask_error_Message, functionNotFound)
{
    assertMessage("function abcd not found", Message::functionNotFound(pos, "abcd"));
}

TEST_F(rask_error_Message, missingSemicolon)
{
    assertMessage("missing \';\'", Message::missingSemicolon(pos));
}

TEST_F(rask_error_Message, uninitializedVariable)
{
    assertMessage("uninitialized variable 'abc'", Message::uninitializedVariable(pos, "abc"));
}

TEST_F(rask_error_Message, redefinition)
{
    assertMessage("redefinition of abc", Message::redefinition(pos, "abc"));
}

TEST_F(rask_error_Message, previousDefinition)
{
    assertMessage("abc previously defined here", Message::previousDefinition(pos, "abc"));
}
