//
// Copyright Somani Software Construction Inc. 2010
//

#include "UserInterface.hpp"

void
UserInterface::execute(boost::function<void()> const & fn)
{
  fn();
}
