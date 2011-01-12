//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_BASIC_TYPE_HPP
#define INCLUDED_BASIC_TYPE_HPP

#include "di/traits/as_type_key.hpp"
#include "di/traits/remove_ptr.hpp"
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace di
{
  namespace traits
  {

    template<typename T>
    struct basic_type
    {
      typedef typename as_type_key<T>::type::type           key;
      typedef typename remove_ptr<key>::type                no_ptr;
      typedef typename boost::remove_cv<no_ptr>::type       no_cv;
      typedef typename boost::remove_reference<no_cv>::type type;
    };
       
  }
}

#endif // BASIC_TYPE_HPP
