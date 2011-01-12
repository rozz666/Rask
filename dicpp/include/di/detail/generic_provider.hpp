//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_GENERIC_PROVIDER_HPP
#define INCLUDED_GENERIC_PROVIDER_HPP

#include <boost/noncopyable.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/static_assert.hpp>
#include <boost/mpl/int.hpp>

#include <boost/preprocessor/repetition/repeat_from_to.hpp>

#include "di/type_provider.hpp"
#include "di/registry_decl.hpp"

#define DI_MAX_ARGS BOOST_FT_MAX_ARITY

namespace di
{
  namespace detail
  {

    template<typename T>
    struct generic_provider_with_instance :
      public typed_provider<T>
    {
      generic_provider_with_instance(boost::shared_ptr<T> i):
        m_i(i)
      {}

      virtual
      boost::shared_ptr<T>
      get() const
      { return m_i; }

    private:
      boost::shared_ptr<T> m_i;
    };

    template<typename Interface, typename Implementation, typename Constructor>
    struct generic_provider :
      public typed_provider<Interface>
    {
      generic_provider(registry & r)
        : m_r(&r)
      {
      }

      virtual
      boost::shared_ptr<Interface>
      get() const
      {
        return construct_helper();
      }

    private:
      boost::shared_ptr<Interface>
      construct_helper() const
      {
        typedef boost::mpl::int_<
        boost::function_types::function_arity<Constructor>::value
          > arity;
      
        return constructor_helper<Implementation>(arity());
      }

      template<typename Impl>
      boost::shared_ptr<Interface>
      constructor_helper(boost::mpl::int_<0>) const
      {
        return boost::shared_ptr<Interface>(new Impl());
      }

      template<typename S, int N>
      struct at_helper
      {
        typedef typename boost::mpl::at<S,boost::mpl::int_<N> >::type type;
      };
      
#define DI__GENERATE_CONSTRUCTOR_ARG(N) m_r->construct<typename at_helper<parameters,N>::type>()
      
#define DI__GENERATE_CONSTRUCTOR_ARG1(z,N,t) DI__GENERATE_CONSTRUCTOR_ARG(N) ,
      
#define DI__GENERATE_CONSTRUCTOR_HELPER(N)                              \
      template<typename Impl>                                           \
      boost::shared_ptr<Interface>                                      \
      constructor_helper(boost::mpl::int_<N>) const                     \
      {                                                                 \
        typedef typename                                                \
          boost::function_types::parameter_types<Constructor>::type parameters; \
                                                                        \
        return boost::shared_ptr<Interface>(                            \
          new Impl(                                                     \
            BOOST_PP_REPEAT(BOOST_PP_DEC(N),DI__GENERATE_CONSTRUCTOR_ARG1,~) \
            DI__GENERATE_CONSTRUCTOR_ARG(BOOST_PP_DEC(N))               \
            )                                                           \
          );                                                            \
      }
      
#define DI__GENERATE_CONSTRUCTOR_HELPER_REPEAT(z,N,t)   \
      DI__GENERATE_CONSTRUCTOR_HELPER(N)

      BOOST_PP_REPEAT_FROM_TO(1,DI_MAX_ARGS,DI__GENERATE_CONSTRUCTOR_HELPER_REPEAT,~)

    private:
      registry *m_r;
    };

  }
}

#endif // GENERIC_PROVIDER_HPP
