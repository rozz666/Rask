//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_CONSTRUCTOR_HPP
#define INCLUDED_CONSTRUCTOR_HPP

#include "di/type_key.hpp"

#define DI_CONSTRUCTOR_TYPES(Signature)                         \
  typedef di::type_key< void Signature >::type constructor

#define DI_CONSTRUCTOR(Type,Signature)                          \
  DI_CONSTRUCTOR_TYPES(Signature);                              \
  Type Signature

#endif // CONSTRUCTOR_HPP
