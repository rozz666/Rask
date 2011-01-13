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

namespace rask
{

class InputStream
{
    typedef std::vector<char> Buffer;

public:

    typedef Buffer::const_iterator const_iterator;

    InputStream(const std::string& file, std::istream& stream);

    std::string file() const { return file_; }
    const_iterator begin() const { return buffer_.begin(); }
    const_iterator end() const { return buffer_.end(); }

private:

    std::string file_;
    Buffer buffer_;
};

}

#endif /* RASK_INPUTSTREAM_HPP */