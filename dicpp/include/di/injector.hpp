//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_INJECTOR_HPP
#define INCLUDED_INJECTOR_HPP

#include <boost/function.hpp>
#include "di/registry.hpp"
#include "di/traits/returned_type.hpp"

namespace di
{
  
  struct injector
  {
  private:
    registry m_r;

  public:
    injector&
    install(boost::function<void(registry & r)> const & register_types)
    {
      register_types(m_r);
      return *this;
    }

    template<typename T>
    typename traits::returned_type<T>::type
    construct() const
    {
      return m_r.construct<T>();
    }

  };

}

#endif // INJECTOR_HPP
