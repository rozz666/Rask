#ifndef RASK_POSITION_HPP
#define RASK_POSITION_HPP

#include <iosfwd>
#include <string>
#include <boost/flyweight.hpp>

namespace rask
{

struct Position
{
    boost::flyweight<std::string> file;
    unsigned row;
    unsigned column;

    Position() : row(0), column(0) { }
    explicit Position(const std::string& file, unsigned row = 0, unsigned column = 0) : file(file), row(row), column(column) { }
};

inline bool operator==(const Position& left, const Position& right)
{
    return left.row == right.row && left.column == right.column && left.file == right.file;
}

inline bool operator!=(const Position& left, const Position& right)
{
    return !(left == right);
}

inline bool operator<(const Position& left, const Position& right)
{
    return
        left.file < right.file ||
        (left.file == right.file && (left.row < right.row || 
            (left.row == right.row && left.column < right.column))); 
}

std::ostream& operator<<(std::ostream& os, const Position& p);

}

#endif /* RASK_POSITION_HPP */