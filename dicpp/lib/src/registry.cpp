//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"

di::registry::registry()
{
  // bootstrap no_scope
  add_scope<scopes::no_scope>();
}

