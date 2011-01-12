//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/scopes/singleton.hpp"
#include "di/detail/demangle.hpp"
#include "di/logging.hpp"

boost::shared_ptr<void>
di::scopes::singleton::construct( di::type_info const & type,
                                  di::type_provider & unscoped )
{
  boost::recursive_mutex::scoped_lock lock(m_mutex);
  {
    instance_map_t::iterator it = m_instances.find(type);
    if(it == m_instances.end())
    {
      DI_LOG_DEBUG("Singleton: constructing: " << detail::demangle(type.name()));
      m_instances[type] = unscoped.untyped_get();
    }
    else
    {
      DI_LOG_DEBUG("Singleton: returning existing: " << detail::demangle(type.name()));
    }
    return m_instances[type];
  }
}
