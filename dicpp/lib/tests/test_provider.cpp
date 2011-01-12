//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"
#include "di/constructor.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( TypedProvider )
{
  di::registry r;

  boost::shared_ptr<di::typed_provider<int> > int_provider;
  int_provider.reset(
    new di::detail::generic_provider<int,int,void()>(r)
    );

  BOOST_REQUIRE_EQUAL(*(int_provider->get()),int());
}

BOOST_AUTO_TEST_CASE( ProviderRegistration )
{
  di::registry r;

  r.add(
    r.type<int>()
    );

  boost::shared_ptr<di::typed_provider<int> > provider1(
    r.construct<boost::shared_ptr<di::typed_provider<int> > >()
    );

  boost::shared_ptr<di::typed_provider<int> > provider2(
    r.construct<boost::shared_ptr<di::typed_provider<int> > >()
    );

  BOOST_REQUIRE_EQUAL(provider1,provider2);
  BOOST_REQUIRE_EQUAL(*provider1->get(),*provider2->get());
}

struct Provided
{
  static std::size_t s_instances;

  Provided()
  {
    ++s_instances;
  }

};

std::size_t Provided::s_instances = 0;

struct Client
{
  Client(di::typed_provider<Provided> & p)
    :m_p(&p)
  {}

  Provided
  get()
  {
    return *m_p->get();
  }

private:
  di::typed_provider<Provided> *m_p;
};

namespace di
{
  namespace traits
  {

    template<>
    struct type_meta<Client>
    {
      typedef type_key<void(di::typed_provider<Provided> &)>::type constructor;
    };

  }
}

BOOST_AUTO_TEST_CASE( ProviderInjection )
{
  Provided::s_instances = 0;

  di::registry r;
  r.add(
    r.type<Client>()
    );

  r.add(
    r.type<Provided>()
    );

  Client c = r.construct<Client>();
  BOOST_REQUIRE_EQUAL(Provided::s_instances,0u);

  c.get();
  BOOST_REQUIRE_EQUAL(Provided::s_instances,1u);
}

template<int N>
struct ProviderParameter
{
  enum V { value = N };
};

class CustomProvider : public di::typed_provider<Provided>
{
public:
  static std::size_t s_instances_created;

  DI_CONSTRUCTOR(CustomProvider,
                 (ProviderParameter<0> a,
                  ProviderParameter<1> b,
                  ProviderParameter<2> c))
  {
    BOOST_REQUIRE_EQUAL(a.value,0);
    BOOST_REQUIRE_EQUAL(b.value,1);
    BOOST_REQUIRE_EQUAL(c.value,2);
  }

  boost::shared_ptr<Provided>
  get() const
  {
    ++s_instances_created;

    return boost::shared_ptr<Provided>(
      new Provided
      );
  }
};

std::size_t CustomProvider::s_instances_created = 0;

BOOST_AUTO_TEST_CASE( CustomProviderTest )
{
  Provided::s_instances               = 0;
  CustomProvider::s_instances_created = 0;

  di::registry r;

  r.add(
    r.type<Provided>()
    .provider<CustomProvider>()
    );

  BOOST_REQUIRE_EQUAL(CustomProvider::s_instances_created,0U);

  r.construct<Provided>();

  BOOST_REQUIRE_EQUAL(Provided::s_instances,1U);
  BOOST_REQUIRE_EQUAL(CustomProvider::s_instances_created,1U);
}
