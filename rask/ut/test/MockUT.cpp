// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h>
#include <rask/test/TUTAssert.hpp>
#include <rask/test/Mock.hpp>

namespace tut
{

class Mockable
{
public:

    virtual void testFunc(int arg) = 0;
    virtual void testFunc2(int arg1, float arg2) = 0;
    virtual void testFunc3() = 0;
    virtual int testFunc4() = 0;
    virtual void testFunc5(int& x) = 0;
};
    
MOCK(Mock, Mockable)
{
public:

    MOCK_METHOD(void, testFunc, (int, arg));
    MOCK_METHOD(void, testFunc2, (int, arg1)(float, arg2));
    MOCK_METHOD(void, testFunc3, );
    MOCK_METHOD(int, testFunc4, );
    MOCK_METHOD(void, testFunc5, (int&, x));
};

    
struct Mock_TestData
{
    Mock mock;
    Mockable& mockable;

    Mock_TestData() : mockable(mock) { }
};

typedef test_group<Mock_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.test.Mock");
}

namespace tut
{

    
template <>
template <>
void object::test<1>()
{
    ENSURE_EQUALS(mock.testFunc__.calls.size(), 0u);
    
    mockable.testFunc(5);

    ENSURE_EQUALS(mock.testFunc__.calls.size(), 1u);
    ENSURE_EQUALS(mock.testFunc__.calls[0].callIndex__, 1u);
    ENSURE_EQUALS(mock.testFunc__.calls[0].arg, 5);

    mockable.testFunc(10);

    ENSURE_EQUALS(mock.testFunc__.calls.size(), 2u);
    ENSURE_EQUALS(mock.testFunc__.calls[1].callIndex__, 2u);
    ENSURE_EQUALS(mock.testFunc__.calls[1].arg, 10);
}

template <>
template <>
void object::test<2>()
{
    mockable.testFunc2(1, 2);
    
    ENSURE_EQUALS(mock.testFunc2__.calls.size(), 1u);
    ENSURE_EQUALS(mock.testFunc2__.calls[0].callIndex__, 1u);
    ENSURE_EQUALS(mock.testFunc2__.calls[0].arg1, 1);
    ENSURE_EQUALS(mock.testFunc2__.calls[0].arg2, 2);
    
    mockable.testFunc(3);
    
    ENSURE_EQUALS(mock.testFunc__.calls.size(), 1u);
    ENSURE_EQUALS(mock.testFunc__.calls[0].callIndex__, 2u);
    ENSURE_EQUALS(mock.testFunc__.calls[0].arg, 3);
}

template <>
template <>
void object::test<3>()
{
    mockable.testFunc3();
    
    ENSURE_EQUALS(mock.testFunc3__.calls.size(), 1u);
    ENSURE_EQUALS(mock.testFunc3__.calls[0].callIndex__, 1u);
}

template <>
template <>
void object::test<4>()
{
    MOCK_RETURN(mock, testFunc4, 5);
    MOCK_RETURN(*&mock, testFunc4, 6);
    
    ENSURE_EQUALS(mockable.testFunc4(), 5);
    ENSURE_EQUALS(mockable.testFunc4(), 6);
    ENSURE_EQUALS(mockable.testFunc4(), 6);
}

template <>
template <>
void object::test<5>()
{
    int x = 10;

    mockable.testFunc5(x);

    ENSURE_EQUALS(mock.testFunc5__.calls.size(), 1u);
    ENSURE_EQUALS(mock.testFunc5__.calls[0].callIndex__, 1u);
    ENSURE(&mock.testFunc5__.calls[0].x == &x);
}

template <>
template <>
void object::test<6>()
{
    ENSURE_THROWS(ENSURE_CALL(*&mock, testFunc(1)), tut::failure);
}
    
template <>
template <>
void object::test<7>()
{
    mock.testFunc2(1, 2);
    mock.testFunc(1);

    ENSURE_THROWS(ENSURE_CALL(*&mock, testFunc(1)), tut::failure);
}
    
template <>
template <>
void object::test<8>()
{
    mock.testFunc(1);
    
    ENSURE_CALL(mock, testFunc(1));
}

template <>
template <>
void object::test<9>()
{
    mock.testFunc(1);
    
    ENSURE_THROWS(ENSURE_CALL(mock, testFunc(2)), tut::failure);
}

template <>
template <>
void object::test<10>()
{
    int x = 5;
    int y = x;
    
    mock.testFunc5(y);
    
    ENSURE_THROWS(ENSURE_CALL(mock, testFunc5(x)), tut::failure);
}

template <>
template <>
void object::test<11>()
{
    ENSURE_THROWS(mockable.testFunc4(), tut::failure);
}

template <>
template <>
void object::test<12>()
{
    ENSURE_NO_CALLS(mock, testFunc3);
    
    mock.testFunc3();
    
    ENSURE_THROWS(ENSURE_NO_CALLS(*&mock, testFunc3), tut::failure);
}

}
