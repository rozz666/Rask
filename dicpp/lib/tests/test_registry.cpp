//
// Copyright Somani Software Construction Inc. 2010
//

#include "di/registry.hpp"
#include "di/scopes/singleton.hpp"
#include <iostream>

#include <boost/test/unit_test.hpp>

#include <boost/thread/tss.hpp>

class animal
{
public:
  virtual ~animal(){}
  virtual std::string speak() = 0;
};

class cat : public animal
{
  virtual std::string speak(){ return "meow"; }
};

class dog : public animal
{
  virtual std::string speak(){ return "woof!"; }
};

class bird : public animal
{
public:
  virtual std::string speak(){ return "cheep!"; }
};

struct kennel_for_2
{
  kennel_for_2(boost::shared_ptr<animal> animal1,
               boost::shared_ptr<animal> animal2,
               boost::shared_ptr<bird> animal3)
  {
    std::cout << "Animal 1: " << animal1->speak() << std::endl;
    std::cout << "Animal 2: " << animal2->speak() << std::endl;
    std::cout << "Bird: " << animal3->speak() << std::endl;
  }
};

struct feline{};
struct canine{};

namespace di
{
  namespace traits
  {

    template<>
    struct type_meta<kennel_for_2>
    {
      typedef di::type_key<void(type_key<boost::shared_ptr<animal>,feline>,
                                type_key<boost::shared_ptr<animal>,canine>,
                                boost::shared_ptr<bird>)>::type
      constructor;
    };

  }
}

struct thread_local_singleton : public di::scope
{
private:
  typedef std::map<di::type_info,boost::shared_ptr<void> > instance_map_t;
  boost::thread_specific_ptr<instance_map_t> m_instances;

public:
  virtual
  boost::shared_ptr<void>
  construct(di::type_info const & type,
            di::type_provider & unscoped)
  {
    if(!m_instances.get())
      m_instances.reset(new instance_map_t);
    instance_map_t::iterator it = m_instances->find(type);
    if(it == m_instances->end())
      (*m_instances)[type] = unscoped.untyped_get();
    return (*m_instances)[type];
  }
};

BOOST_AUTO_TEST_CASE(Registry)
{
  using di::registry;

  registry r;
  r.add(
    r.type<animal>()
    .annotation<feline>()
    .implementation<cat>()
    );
  
  r.add(
    r.type<animal>()
    .annotation<canine>()
    .implementation<dog>()
    );

  r.add(
    r.type<animal>()
    .instance(new dog)
    );
  
  r.add(
    r.type<kennel_for_2>()
    .in_scope<thread_local_singleton>()
    );

  BOOST_REQUIRE((boost::dynamic_pointer_cast<dog>(r.construct<di::type_key<boost::shared_ptr<animal> ,canine> >() )));
  BOOST_REQUIRE((boost::dynamic_pointer_cast<dog>(r.construct<boost::shared_ptr<animal> >())));

  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
  r.construct<kennel_for_2 &>();
}

struct scope1 : public di::scopes::singleton{};
struct scope2 : public di::scopes::singleton{};

BOOST_AUTO_TEST_CASE( TestRegisteredTwice )
{
  using namespace di;

  registry r;

  r.add(
    r.type<dog>()
    .in_scope<scope1>()
    );

  r.add(
    r.type<dog>()
    .in_scope<scope2>()
    );
  
  BOOST_REQUIRE_EQUAL(di::type_info(di::type_key<scope2>()),
                      r.construction_scope<dog>());
}
