// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef SL_ERR_MESSAGE_HPP
#define SL_ERR_MESSAGE_HPP

#include <iosfwd>
#include <string>
#include <sstream>
#include <boost/preprocessor/seq/enum.hpp>
#include <rask/Position.hpp>

#define RASK_ERROR_MESSAGE(name, args, message) \
static Message name(const Position& pos, BOOST_PP_SEQ_ENUM(args) ) \
{ \
    std::ostringstream os; \
    os << message; \
    return Message(pos, os.str()); \
}

#define RASK_ERROR_MESSAGE_NP(name, message) \
static Message name(const Position& pos) \
{ \
    std::ostringstream os; \
    os << message; \
    return Message(pos, os.str()); \
}
 

namespace rask
{
namespace error
{

class Message
{
public:
    
    Message(const Position& position, const std::string& text) : position_(position), text_(text) { }

    const Position& position() const { return position_; }
    const std::string& text() const { return text_; }

    RASK_ERROR_MESSAGE_NP(missingMainFunction, "missing main function")
    RASK_ERROR_MESSAGE_NP(missingReturnType, "missing return type")
    RASK_ERROR_MESSAGE_NP(missingOpeningBrace, "missing opening brace \'{\'")
    RASK_ERROR_MESSAGE_NP(missingClosingBrace, "missing closing brace \'}\'")
    RASK_ERROR_MESSAGE_NP(missingOpeningParen, "missing opening parenthesis \'(\'")
    RASK_ERROR_MESSAGE_NP(missingClosingParen, "missing closing parenthesis \')\'")
    RASK_ERROR_MESSAGE_NP(missingRightArrow, "missing \'->\'")
    RASK_ERROR_MESSAGE(unknownIdentifier, (const std::string& id), "unknown identifier \'" << id << "\'")
    RASK_ERROR_MESSAGE(functionNotFound, (const std::string& f), "function " << f << " not found")
    RASK_ERROR_MESSAGE_NP(missingSemicolon, "missing \';\'")
    RASK_ERROR_MESSAGE(uninitializedVariable, (const std::string& var), "uninitialized variable \'" << var << "\'")
    RASK_ERROR_MESSAGE(redefinition, (const std::string& what), "redefinition of " << what)
    RASK_ERROR_MESSAGE(previousDefinition, (const std::string& what), what << " previously defined here");

private:

    Position position_;
    std::string text_;
};

inline bool operator==(const Message& left, const Message& right)
{
    return left.position() == right.position() && left.text() == right.text();
}

inline bool operator!=(const Message& left, const Message& right)
{
    return !(left == right);
}

std::ostream& operator<<(std::ostream& os, const Message& m);

}
}

#endif /* SL_ERR_MESSAGE_HPP */