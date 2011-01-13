// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/InputStream.hpp>
#include <iterator>

namespace rask
{

InputStream::InputStream(const std::string& file, std::istream& stream)
    : file_(file), buffer_(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()) { }

}