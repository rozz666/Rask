// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <fstream>
#include <iostream>
#include <iterator>
#include <rask/cg/genFunction.hpp>

int main(int, char **argv)
{
    using namespace rask;

    std::ifstream f(argv[1]);

    f.seekg(0, std::ios::end);

    cg::BytecodeBuffer bb(std::size_t(f.tellg() / sizeof(int)));

    f.seekg(0);
    f.read(reinterpret_cast<char *>(&bb.front()), sizeof(bb[0]) * bb.size());

    std::copy(bb.begin(), bb.end(), std::ostream_iterator<int>(std::cout, "\n"));
}