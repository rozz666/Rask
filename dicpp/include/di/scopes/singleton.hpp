//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_SINGLETON_HPP
#define INCLUDED_SINGLETON_HPP

#include <map>
#include "di/scope.hpp"
#include "boost/thread/recursive_mutex.hpp"

namespace di
{
  namespace scopes
  {
    struct singleton : public di::scope
    {
    private:
      typedef std::map<di::type_info,boost::shared_ptr<void> > instance_map_t;
      instance_map_t         m_instances;
      boost::recursive_mutex m_mutex;

    public:
      virtual
      boost::shared_ptr<void>
      construct(di::type_info const & type,
                di::type_provider & unscoped);

    };
  }

}


#endif // SINGLETON_HPP
