//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_REGISTRY_FWD_HPP
#define INCLUDED_REGISTRY_FWD_HPP

#include <map>
#include <boost/shared_ptr.hpp>

#include "di/detail/registration_builder.hpp"
#include "di/scope.hpp"
#include "di/traits/returned_type.hpp"
#include "di/type_key.hpp"
#include "di/type_provider.hpp"

namespace di
{

  struct registry
  {
    struct construction_info
    {
      construction_info(boost::shared_ptr<type_provider> provider,
                        type_info scope_type):
        provider(provider),
        scope_type(scope_type)
      {}

      construction_info():
        provider(),
        scope_type()
      {}

      boost::shared_ptr<type_provider> provider;
      type_info                        scope_type;
    };

    typedef std::map<di::type_info,construction_info> provider_map_t;
    typedef std::map<di::type_info,boost::shared_ptr<scope> > scope_map_t;

    registry();

    // Entry point into registering a type
    template<typename T>
    detail::registration_builder<T>
    type();

    // Add a type registration based on provided properties
    template<typename T, typename PropertyMap>
    void
    add(detail::registration_builder<T,PropertyMap> const &);

    // Add type T if it is not already added
    template<typename T>
    void
    maybe_add();

    // Add scope T
    template<typename T>
    void
    add_scope();

    // Construct a type
    template<typename T>
    typename traits::returned_type<T>::type
    construct() const;

    // Get construction scope for a given type or no_scope if none
    // found
    template<typename T>
    type_info
    construction_scope() const;

  private:

    provider_map_t m_providers;
    scope_map_t    m_scopes;
  };  

}

#endif // REGISTRY_FWD_HPP
