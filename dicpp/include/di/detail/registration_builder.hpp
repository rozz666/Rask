//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRATION_BUILDER_HPP
#define INCLUDED_REGISTRATION_BUILDER_HPP

#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include "di/traits/type_meta.hpp"
#include "di/typed_provider.hpp"
#include "di/scope.hpp"
#include "di/scopes/no_scope.hpp"

namespace di
{
  struct registry;

  namespace detail
  {
    namespace tags
    {

#     define DI__BUILDER_TAG(name) struct name {}

      DI__BUILDER_TAG(annotation);
      DI__BUILDER_TAG(implementation);
      DI__BUILDER_TAG(instance);
      DI__BUILDER_TAG(in_scope);
      DI__BUILDER_TAG(constructor);
      DI__BUILDER_TAG(provider);
    }

    struct null_deleter
    {
      template<typename T>
      void
      operator()(T*)
      {}
    };

    template<
      typename T,
      typename Properties = boost::mpl::map<>
      >
    struct registration_builder
    {
    public:
      explicit registration_builder(registry & r)
        : m_r(&r)
      {}

    private:
      template<typename T1, typename Properties1>
      friend struct registration_builder;

      template<typename T1, typename Properties1>
      registration_builder(registration_builder<T1,Properties1> const & r) :
        m_r(r.m_r),
        m_instance(r.m_instance)
      {}

    public:

      template<typename T1, typename Properties1,
               typename NewPropertyTag, typename NewProperty>
      struct new_type
      {
        typedef typename boost::mpl::insert<Properties1,
                                            boost::mpl::pair<NewPropertyTag,
                                                             NewProperty> >::type
        new_props;

        typedef registration_builder<T1,new_props> type;
      };

      template<typename NewPropertyTag, typename NewProperty>
      typename new_type<T,Properties,NewPropertyTag,NewProperty>::type
      next_step()
      {
        return typename new_type<T,Properties,NewPropertyTag,NewProperty>::type(*this);
      }

      template<typename Annotation>
      typename new_type<T,Properties,tags::annotation,Annotation>::type
      annotation()
      {
        return next_step<tags::annotation,Annotation>();
      }

      template<typename Impl>
      typename new_type<T,Properties,tags::implementation,Impl>::type
      implementation()
      {
        BOOST_MPL_ASSERT((boost::is_base_of<T,Impl>));

        return next_step<tags::implementation,Impl>();
      }

      template<typename Scope>
      typename new_type<T,Properties,tags::in_scope,Scope>::type
      in_scope()
      {
        BOOST_MPL_ASSERT((boost::is_base_of<di::scope,Scope>));

        return next_step<tags::in_scope,Scope>();
      }

      template<typename ConstructorArguments>
      typename new_type<T,Properties,tags::constructor,ConstructorArguments>::type
      constructor()
      {
        return next_step<tags::constructor,ConstructorArguments>();
      }

      template<typename Impl>
      typename new_type<T,Properties,tags::instance,Impl>::type
      instance(Impl * impl)
      {
        m_instance.reset(impl);
        return next_step<tags::instance,Impl>();
      }

      template<typename Provider>
      typename new_type<T,Properties,tags::provider,Provider>::type
      provider()
      {
        return next_step<tags::provider,Provider>();
      }

      registry             *m_r;
      boost::shared_ptr<T>  m_instance;
    };

    template<typename A, typename K, typename D>
    struct at
    {
      typedef
      typename boost::mpl::has_key<A,K>::type
      has_key;

      typedef
      typename
      boost::mpl::eval_if<has_key,
                          boost::mpl::at<A,K>,
                          boost::mpl::identity<D> >::type
      type;
    };

    template<typename T, typename PropertyMap>
    struct extract_construction_properties
    {
      BOOST_MPL_ASSERT((boost::mpl::is_sequence<PropertyMap>));

      typedef T constructed_type;

      typedef
      typename at<PropertyMap,tags::annotation,void>::type
      annotation;

      typedef
      typename at<PropertyMap,tags::implementation,T>::type
      implementation;

      typedef
      typename at<PropertyMap,tags::in_scope,scopes::no_scope>::type
      scope;

      typedef
      typename at<PropertyMap,tags::constructor,
                  typename traits::type_meta<implementation>::constructor>::type
      constructor;

      typedef
      typename at<PropertyMap,tags::provider,
                  typed_provider<constructed_type> >::type
      provider;
    };

  }
}

#endif // REGISTRATION_BUILDER_HPP
