// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_TUTASSERT_HPP
#define RASK_TEST_TUTASSERT_HPP

#include <tut/tut.hpp>

namespace tut
{

template <typename M, typename Cont, typename Size>
void ensure_size(const M& msg, Cont& cont, Size size)
{
    ensure_equals(msg, cont.size(), size);
}

template <typename M, typename Cont>
void ensure_contains(const M& msg, Cont& cont, typename Cont::iterator::pointer what)
{
    for (typename Cont::iterator it = cont.begin(), end = cont.end();  it != end; ++it)
    {
        if (&*it == what) return;
    }
    
    fail(msg);
}
    
}

#endif /* RASK_TEST_TUTASSERT_HPP */
