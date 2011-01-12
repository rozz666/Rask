//
// Copyright Somani Software Construction Inc. 2010
//

#ifndef INCLUDED_STOCKQUOTEAPP_HPP
#define INCLUDED_STOCKQUOTEAPP_HPP

#include "di/constructor.hpp"
#include "UserInterface.hpp"
#include "StockQuoteService.hpp"

// "Controller"
struct StockQuoteApp
{
  DI_CONSTRUCTOR(StockQuoteApp,
                 (boost::shared_ptr<UserInterface> ui,
                  boost::shared_ptr<StockQuoteServiceFactory> factory));

  void
  execute();

private:
  void
  loop();

  boost::shared_ptr<UserInterface>            m_ui;
  boost::shared_ptr<StockQuoteServiceFactory> m_factory;
};

#endif // STOCKQUOTEAPP_HPP
