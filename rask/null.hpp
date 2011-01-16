// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef RASK_NULL_HPP
#define RASK_NULL_HPP

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>

namespace rask
{

class Null
{
public:

    template <typename T>
    operator T *() const { return 0; }

    template <typename T>
    operator boost::shared_ptr<T>() const { return boost::shared_ptr<T>(); }

    template <typename T>
    operator boost::weak_ptr<T>() const { return boost::weak_ptr<T>(); }

    template <typename T>
    operator boost::optional<T>() const { return boost::optional<T>(); }
};

template <typename T>
inline bool operator==(const Null&, T *right)
{
    return right == 0;
}

template <typename T>
inline bool operator!=(const Null&, T *right)
{
    return right != 0;
}

template <typename T>
inline bool operator==(T *left, const Null&)
{
    return left == 0;
}

template <typename T>
inline bool operator!=(T *left, const Null&)
{
    return left != 0;
}

inline bool operator==(const Null&, const Null&)
{
    return true;
}

inline bool operator!=(const Null&, const Null&)
{
    return false;
}

const Null null = Null();

}

#endif // RASK_NULL_HPP
