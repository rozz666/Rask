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
void ensure_empty(const M& msg, Cont& cont)
{
    ensure(msg, cont.empty());
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

inline std::string formatMessage(const char *what, const char *file, int line)
{
    std::ostringstream ss;
    ss << file << ":" << line << ": " << what;
    return ss.str();
}

#define FAIL(msg) fail(formatMessage(msg, __FILE__, __LINE__))
#define ENSURE(x) ensure(formatMessage(#x, __FILE__, __LINE__), (x))
#define ENSURE_EQUALS(expr, expected) ensure_equals(formatMessage(#expr, __FILE__, __LINE__), (expr), (expected))
#define ENSURE_THROWS(expr, ex) \
    do { \
        bool thrown = false; \
        try \
        { \
            expr; \
        } \
        catch (const ex& ) \
        { \
            thrown = true; \
        } \
        if (!thrown) fail(formatMessage(#expr " has not thrown", __FILE__, __LINE__)); \
    } while (0)

}

#endif /* RASK_TEST_TUTASSERT_HPP */
