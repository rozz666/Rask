#ifndef SL_ERR_MESSAGE_HPP
#define SL_ERR_MESSAGE_HPP

#include <iosfwd>
#include <string>
#include <sstream>
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