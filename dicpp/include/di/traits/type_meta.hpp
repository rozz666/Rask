//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_TYPE_META_HPP
#define INCLUDED_TYPE_META_HPP

#include "di/traits/constructor.hpp"

namespace di
{

  namespace traits
  {

    template<typename T>
    struct type_meta
    {
      typedef typename constructor<T>::type constructor;
    };
  }

}

#endif // TYPE_META_HPP
