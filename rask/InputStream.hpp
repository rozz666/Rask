// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_INPUTSTREAM_HPP
#define RASK_INPUTSTREAM_HPP

#include <string>
#include <iosfwd>
#include <vector>
#include <iterator>

namespace rask
{

class InputStream
{
public:

    typedef std::vector<char>::const_iterator const_iterator;

    InputStream(const std::string& file, std::istream& stream);

    const std::string file() const { return file_; }
    const_iterator begin() const { return buffer_.begin(); }
    const_iterator end() const { return buffer_.end(); }

private:

    std::string file_;
    std::vector<char> buffer_;
};

}

#endif /* RASK_INPUTSTREAM_HPP */