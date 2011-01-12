//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRY_HPP
#define INCLUDED_REGISTRY_HPP

#define BOOST_FT_MAX_ARITY 20
#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/static_warning.hpp>

#include "di/detail/demangle.hpp"
#include "di/detail/generic_provider.hpp"
#include "di/detail/registration_builder.hpp"
#include "di/registry_decl.hpp"
#include "di/scope.hpp"
#include "di/scopes/no_scope.hpp"
#include "di/traits/as_type_key.hpp"
#include "di/traits/basic_type.hpp"
#include "di/traits/is_typed_provider.hpp"
#include "di/traits/provided_type.hpp"
#include "di/traits/returned_type.hpp"
#include "di/type_info.hpp"
#include "di/type_provider.hpp"
#include "di/typed_provider.hpp"
#include "di/scoped_provider.hpp"
#include "di/scopes/singleton.hpp"
#include "di/logging.hpp"

namespace di
{
  template<typename Scope>
  void
  registry::add_scope()
  {
    boost::shared_ptr<Scope> new_scope(new Scope);

    m_providers.insert(
      std::make_pair(
        type_key<Scope>(),
        construction_info(
          boost::shared_ptr<type_provider>(
            new detail::generic_provider_with_instance<Scope>(
              new_scope
              )
            ),
          type_key<Scope>()
          )
        )
      );
    m_scopes[type_key<Scope>()] = new_scope;
  }

  template<typename T>
  detail::registration_builder<T>
  registry::type()
  {
    return detail::registration_builder<T>(*this);
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  boost::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry&r,
               boost::shared_ptr<ConstructedType> instance,
               boost::mpl::true_ /* instance provided */,
               boost::mpl::true_ /* default type provider */)
  {
    return
      boost::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider_with_instance<ConstructedType>(instance)
        );
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  boost::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry&r,
               boost::shared_ptr<ConstructedType> instance,
               boost::mpl::false_ /* instance provided */,
               boost::mpl::true_ /* default type provider */)
  {
    return
      boost::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider<ConstructedType,Implementation,Constructor>(r)
        );
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  boost::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry &r,
               boost::shared_ptr<ConstructedType> instance,
               boost::mpl::true_ /* instance provided */,
               boost::mpl::false_ /* default type provider */)
  {
    return
      boost::shared_ptr<typed_provider<ConstructedType> >(
        new detail::generic_provider_with_instance<ConstructedType>(instance)
        );
    return boost::shared_ptr<typed_provider<ConstructedType> >();
  }

  template<typename ConstructedType, typename Implementation, typename Constructor, typename Provider>
  boost::shared_ptr<typed_provider<ConstructedType> >
  get_provider(registry &r,
               boost::shared_ptr<ConstructedType> instance,
               boost::mpl::false_ /* instance provided */,
               boost::mpl::false_ /* default type provider */)
  {
    r.maybe_add<Provider>();
    return r.construct<boost::shared_ptr<Provider> >();
  }

  namespace detail
  {

    template<typename T>
    void
    maybe_add_provided_type( registry &, boost::mpl::false_ )
    {
    }

    template<typename T>
    void
    maybe_add_provided_type( registry & r, boost::mpl::true_ )
    {
      r.maybe_add<typename traits::provided_type<T>::type>();
    }

    struct add_helper
    {

      typedef void result_type;

      template<typename A1=void, typename A2=void>
      struct result
      { typedef result_type type; };

      template<typename ArgT>
      void
      operator()(registry & r, ArgT const *) const
      {
        typedef typename traits::basic_type<ArgT>::type T;

        // If the type is a type provider, make sure that the provided type is registered
        typedef typename traits::is_typed_provider<T>::type is_provider;
        detail::maybe_add_provided_type<T>(r,is_provider());

        r.maybe_add<T>();
      }

    };

    template<typename T>
    void
    do_add(registry & r, boost::mpl::false_/* is abstract */)
    {
      r.add(r.type<T>());
    }

    template<typename T>
    void
    do_add(registry & r, boost::mpl::true_ /* is abstract */)
    {
      // We don't add abstract classes because we need to be told how
      // to construct them.
    }

  }

  template<typename T>
  void
  registry::maybe_add()
  {
    if(m_providers.find(type_key<T>()) == m_providers.end())
    {
      typedef typename boost::is_abstract<T>::type is_abstract;
      detail::do_add<T>(*this,is_abstract());
    }
  }

  template<typename T, typename PropertyMap>
  void
  registry::add(detail::registration_builder<T,PropertyMap> const & r)
  {
    typedef typename detail::extract_construction_properties<T,PropertyMap> properties;

    typedef typename properties::constructed_type constructed_type;
    typedef typename properties::annotation       annotation;
    typedef typename properties::implementation   implementation;
    typedef typename properties::scope            construction_scope;
    typedef typename properties::constructor      constructor;
    typedef typename properties::provider         custom_provider;

    DI_LOG_DEBUG("Registering: " << detail::demangle(typeid(constructed_type).name()));

    // Recursively register parameters
    typedef typename boost::function_types::parameter_types<constructor>::type parameters;
    typedef typename boost::mpl::transform<parameters,boost::add_pointer<boost::mpl::_1> >::type pointer_parameters;
    boost::mpl::for_each<pointer_parameters>(boost::bind(detail::add_helper(),
                                                         boost::ref(*this),
                                                         _1));

    // construct the provider. The fudging around with provided
    // instance is to avoid instantiating code for abstract classes.
    typedef typename
      boost::mpl::has_key<PropertyMap,detail::tags::instance>::type
      provided_instance;

    typedef typename
      boost::is_same<typed_provider<constructed_type>,custom_provider>::type
      provided_provider;

    boost::shared_ptr<typed_provider<constructed_type> > unscoped_provider(
      get_provider<constructed_type,implementation,constructor,custom_provider>(*this,r.m_instance,
                                                                                provided_instance(),
                                                                                provided_provider())
      );

    // Ensure the required scope is available
    if(m_scopes.find(type_info(type_key<construction_scope>())) == m_scopes.end())
      add_scope<construction_scope>();

    scope_map_t::iterator scope_it = m_scopes.find(type_info(type_key<construction_scope>()));

    // Construct the scoped_provider (decorator pattern)
    boost::shared_ptr<typed_provider<constructed_type> > scoped_prov(
      new scoped_provider<constructed_type>(
        unscoped_provider,
        *scope_it->second
        )
      );

    // Note: construction key requires annotation
    m_providers[type_key<constructed_type,annotation>()] = construction_info(scoped_prov,type_key<construction_scope>());

    // Now add the provider for provider injection.
    typedef typed_provider<constructed_type> provider_type;
    typedef typed_provider<provider_type> provider_provider_type;
    boost::shared_ptr<provider_provider_type> provider_provider(
      new detail::generic_provider_with_instance<provider_type>(scoped_prov)
      );

    m_providers[type_key<provider_type,annotation>()] = construction_info(provider_provider,type_key<scopes::no_scope>());
  }

  template<typename T>
  type_info
  registry::construction_scope() const
  {
    typedef typename traits::as_type_key<T>::type T_type_key;
    provider_map_t::const_iterator it = m_providers.find(T_type_key());

    if(it == m_providers.end())
      return type_key<scopes::no_scope>();
    else
      return it->second.scope_type;
  }

  namespace detail
  {

    template<typename T>
    struct do_cast
    {
      static
      T
      doit(boost::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<T const &>
    {
      static
      T const &
      doit(boost::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<T &>
    {
      static
      T &
      doit(boost::shared_ptr<T> const & v)
      {
        return *v;
      }
    };

    template<typename T>
    struct do_cast<boost::shared_ptr<T> >
    {
      static
      boost::shared_ptr<T>
      doit(boost::shared_ptr<T> const & v)
      {
        return v;
      }
    };

  }

  template<typename T>
  typename traits::returned_type<T>::type
  registry::construct() const
  {
    typedef type_key<typename traits::basic_type<typename traits::as_type_key<T>::type>::type,
      typename traits::as_type_key<T>::type::annotation> T_type_key;

    DI_LOG_DEBUG("Constructing: " << detail::demangle(typeid(T_type_key).name()));

    provider_map_t::const_iterator pit = m_providers.find(T_type_key());
    if(pit == m_providers.end())
    {
      std::string message = "Requested type without corresponding provider: " +
        detail::demangle(typeid(T_type_key).name());
      throw std::runtime_error(message.c_str());
    }

    typedef typename T_type_key::type     provided_type;
    typedef typed_provider<provided_type> provider_type;

    boost::shared_ptr<provider_type> provider =
      boost::dynamic_pointer_cast<provider_type>(pit->second.provider);

    if(!provider.get())
      throw std::runtime_error("Provider is of incorrect type");

    boost::shared_ptr<provided_type> instance = provider->get();

    DI_LOG_DEBUG("Completed constructing: " << detail::demangle(typeid(T_type_key).name()) << " with address: " << std::hex << instance.get());

    return detail::do_cast<typename traits::returned_type<T>::type>::doit(instance);
  }

}


#endif // REGISTRY_HPP
