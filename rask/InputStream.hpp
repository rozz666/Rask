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

namespace rask
{

class InputStream
{
public:

    InputStream(const std::string& file, std::istream& stream) : file_(file), stream_(stream) { }

    const std::string file() const { return file_; }
    std::istream& stream() { return stream_; }

private:

    std::string file_;
    std::istream& stream_;
};

}

#endif /* RASK_INPUTSTREAM_HPP */