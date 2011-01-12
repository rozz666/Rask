//
// Copyright Somani Software Construction Inc. (2010)
//

#ifndef INCLUDED_SCOPED_PROVIDER_HPP_08012010
#define INCLUDED_SCOPED_PROVIDER_HPP_08012010

#include "di/typed_provider.hpp"
#include "di/traits/as_type_key.hpp"

namespace di
{

  template<typename T>
  struct scoped_provider :
    public typed_provider<T>
  {
    scoped_provider(boost::shared_ptr<typed_provider<T> > & unscoped,
                    scope & scope):
      m_unscoped(unscoped),
      m_scope(scope)
    {}

    virtual
    boost::shared_ptr<T>
    get() const
    {
      return boost::static_pointer_cast<T>(
        m_scope.construct(
          typename traits::as_type_key<T>::type(),
          *m_unscoped)
        );
    }

  private:
    boost::shared_ptr<typed_provider<T> > m_unscoped;
    scope                               & m_scope;
  };

}

#endif // INCLUDED_SCOPED_PROVIDER_HPP_08012010
