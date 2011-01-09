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
    virtual short testConstFunc(int arg1, const float& arg2) const = 0;
    virtual void testFunc6(int& x) = 0;
    virtual void testFunc7(const float& x) const = 0;
    virtual int testFunc8(int x, const float& y, float& z) const = 0;
    virtual int& testFunc9() const = 0;
    virtual int& testFunc10(int x) const = 0;
    virtual int testFunc11(int x) = 0;
};

CLASS_MOCK(Mock, Mockable)
{
    MOCK_METHOD(void, testFunc, (int, arg))
    MOCK_METHOD(void, testFunc2, (int, arg1)(float, arg2))
    MOCK_METHOD(void, testFunc3, )
    MOCK_METHOD(int, testFunc4, )
    MOCK_METHOD(void, testFunc5, (int&, x))
    MOCK_CONST_METHOD(short, testConstFunc, (int, arg1)(const float& , arg2))
    MOCK_METHOD_TRANSFORM(void, testFunc6, (int&, x), (int, x))
    MOCK_CONST_METHOD_TRANSFORM(void, testFunc7, (const float&, x), (float, x))
    MOCK_CONST_METHOD_TRANSFORM(int, testFunc8, (int, x)(const float&, y)(float&, z), (int, x)(float, y)(float&, z))
    MOCK_CONST_METHOD(int&, testFunc9, )
    MOCK_CONST_METHOD(int&, testFunc10, (int, x))
    MOCK_METHOD(int, testFunc11, (int, x))
};


struct Mock_TestData
{
    Mock mock;
    const Mock& cmock;
    Mockable& mockable;

    Mock_TestData() : cmock(mock), mockable(mock) { }
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
    ENSURE_THROWS(mockable.testFunc4(), tut::failure);
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

template <>
template <>
void object::test<13>()
{
    ENSURE_NO_CALLS(mock, testConstFunc);

    short x = 7;
    MOCK_RETURN(mock, testConstFunc, x);

    int y = 9;
    float z;

    ENSURE_EQUALS(mock.testConstFunc(y, z), x);
    ENSURE_CALL(mock, testConstFunc(y, z));
}

template <>
template <>
void object::test<14>()
{
    int x = 5;
    int y = x;

    mock.testFunc6(y);

    ENSURE_CALL(mock, testFunc6(x));
}

template <>
template <>
void object::test<15>()
{
    float x = 5;
    int y = x;

    cmock.testFunc7(y);

    ENSURE_CALL(cmock, testFunc7(x));
}

template <>
template <>
void object::test<16>()
{
    int x1 = 10;
    int x2 = 17;
    int x3 = 19;
    float y1 = 3.3f;
    float y2 = 4.4f;
    float y3 = 4.5f;
    float z1;
    float z2;
    float z3;
    int r1 = 2;
    int r2 = 3;
    int r3 = 9;

    MOCK_RETURN(cmock, testFunc8(x1, y1, z1), r1);
    MOCK_RETURN(cmock, testFunc8(x2, y2, z2), r2);
    MOCK_RETURN(cmock, testFunc8(x3, y3, z3), r3);

    ENSURE_EQUALS(cmock.testFunc8(x2, y2, z2), r2);
    ENSURE_EQUALS(cmock.testFunc8(x3, y3, z3), r3);
    ENSURE_EQUALS(cmock.testFunc8(x1, y1, z1), r1);

    ENSURE_NO_CALLS(mock, testFunc8);
}

template <>
template <>
void object::test<17>()
{
    int x = 8;
    int i1;
    int i2;

    MOCK_RETURN(cmock, testFunc9, i1);
    MOCK_RETURN(cmock, testFunc10(x), i2);

    ENSURE(&cmock.testFunc9() == &i1);
    ENSURE(&cmock.testFunc10(x) == &i2);
}

template <>
template <>
void object::test<18>()
{
    int x1 = 8;
    int v1 = 10;
    int v2 = 1;
    int v3 = 996;
    int v4 = 47;
    int v5 = 835;

    MOCK_RETURN(mock, testFunc11(x1), v1);
    MOCK_RETURN(mock, testFunc11(x1), v2);
    MOCK_RETURN(mock, testFunc11(x1), v3);
    MOCK_RETURN(mock, testFunc11, v4);
    MOCK_RETURN(mock, testFunc11, v5);

    ENSURE(mock.testFunc11(x1) == v1);
    ENSURE(mock.testFunc11(x1) == v2);
    ENSURE(mock.testFunc11(100) == v4);
    ENSURE(mock.testFunc11(200) == v5);
    ENSURE(mock.testFunc11(x1) == v3);
    ENSURE_THROWS(mock.testFunc11(x1), tut::failure);
}

template <>
template <>
void object::test<19>()
{
    int arg = 8;
    MOCK_RETURN(mock, testFunc11(arg), 1);
    mock.testFunc11(arg);
    MOCK_VERIFY(mock);
}

template <>
template <>
void object::test<20>()
{
    int arg1 = 8, arg2 = 19;
    MOCK_RETURN(mock, testFunc11(arg1), 1);
    MOCK_RETURN(mock, testFunc11(arg2), 2);
    MOCK_RETURN(mock, testFunc11(arg2), 3);
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc11(arg1) not called");
    mock.testFunc11(arg1);
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc11(arg2) not called");
    mock.testFunc11(arg2);
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc11(arg2) not called");
    mock.testFunc11(arg2);
    MOCK_VERIFY(mock);
}

template <>
template <>
void object::test<21>()
{
    MOCK_RETURN(mock, testFunc4, 1);
    MOCK_RETURN(mock, testFunc4, 2);
    MOCK_RETURN(mock, testFunc4, 3);
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc4 not called");
    mock.testFunc4();
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc4 not called");
    mock.testFunc4();
    ENSURE_THROWS_WITH_MESSAGE(MOCK_VERIFY(mock), tut::failure, "testFunc4 not called");
    mock.testFunc4();
    MOCK_VERIFY(mock);
}

}
