// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_TEST_MOCK_HPP
#define RASK_TEST_MOCK_HPP

#include <boost/ptr_container/ptr_deque.hpp>
#include <queue>
#include <boost/preprocessor.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace rask
{
namespace test
{

class MockBase
{
public:
    
    MockBase() : ci(0), vi(0) { }

    unsigned getCallIndex() { return ++ci; }
    unsigned getCallVerificationIndex() { return ++vi; }

private:

    unsigned ci;
    unsigned vi;
};

#define MOCK(name, parent) class name : public parent, public ::rask::test::MockBase

#define MOCK_METHOD_FILLER_0(X, Y) \
    ((X, Y)) MOCK_METHOD_FILLER_1
#define MOCK_METHOD_FILLER_1(X, Y) \
    ((X, Y)) MOCK_METHOD_FILLER_0
#define MOCK_METHOD_FILLER_0_END
#define MOCK_METHOD_FILLER_1_END
    
#define MOCK_DECLARE_MEMBER(r, data, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem) ;
#define MOCK_DECLARE_ARG(r, data, i, elem) BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem)
#define MOCK_DECLARE_ARG_NC(r, data, elem) , BOOST_PP_TUPLE_ELEM(2, 0, elem) BOOST_PP_TUPLE_ELEM(2, 1, elem)
#define MOCK_DECLARE_ARG_TYPE(r, data, i, elem) BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(2, 0, elem)
#define MOCK_ENUM_ARG(r, data, elem) , BOOST_PP_TUPLE_ELEM(2, 1, elem)
#define MOCK_TEST_ARG(r, data, elem) \
if (!Test<BOOST_PP_TUPLE_ELEM(2, 0, elem)>::equal(BOOST_PP_TUPLE_ELEM(2, 1, elem), call.BOOST_PP_TUPLE_ELEM(2, 1, elem))) \
{ \
    std::ostringstream ss; \
    ss << "Mismatched parameter arg: expected arg "; \
    fail(formatMessage(ss.str().c_str(), file, line)); \
}

#define MOCK_DECL_MEMBERS(vars) \
    BOOST_PP_SEQ_FOR_EACH(MOCK_DECLARE_MEMBER, _, vars)

#define MOCK_DECL_ARGS(args) \
    BOOST_PP_SEQ_FOR_EACH_I(MOCK_DECLARE_ARG, _, args)

#define MOCK_DECL_ARG_TYPES(args) \
BOOST_PP_SEQ_FOR_EACH_I(MOCK_DECLARE_ARG_TYPE, _, args)

#define MOCK_DECL_ARGS_NC(args) \
    BOOST_PP_SEQ_FOR_EACH(MOCK_DECLARE_ARG_NC, _, args)

#define MOCK_ENUM_ARGS(args) \
    BOOST_PP_SEQ_FOR_EACH(MOCK_ENUM_ARG, _, args)

#define MOCK_TEST_ARGS(args) \
    BOOST_PP_SEQ_FOR_EACH(MOCK_TEST_ARG, _, args)

#define MOCK_METHOD(retType, name, args) \
struct name##__type { \
    typedef typename boost::mpl::if_<boost::is_void<retType>, int, retType>::type retValueType; \
    struct Call \
    { \
        unsigned callIndex__;\
        MOCK_DECL_MEMBERS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))\
        struct Verifier \
        { \
            const char *file; \
            int line; \
            Call& call; \
            Verifier(const char *file, int line, Call& call) : file(file), line(line), call(call) { } \
            \
            template <typename T, bool ref = boost::is_reference<T>::value> \
            struct Test { \
                static bool equal(const T& left, const T& right) { return left == right; } \
            }; \
            \
            template <typename T> \
            struct Test<T, true> { \
                static bool equal(const T& left, const T& right) { return &left == &right; } \
            }; \
            \
            void name(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) \
            { \
                MOCK_TEST_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) \
            } \
        }; \
        Verifier verify__(const char *file, int line, Call& call) \
        { \
            return Verifier(file, line, call); \
        } \
    }; \
    boost::ptr_deque<Call> calls; \
    std::queue<retValueType> retValues; \
} name##__; \
name##__type& call__##name(MOCK_DECL_ARG_TYPES(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) { return name##__; } \
retType name(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) \
{ \
    name##__type::Call c = { getCallIndex() MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) }; \
    name##__.calls.push_back(new name##__type::Call(c)); \
    name##__type::retValueType ret = name##__.retValues.front(); \
    name##__.retValues.pop();\
    return (retType) ret; \
}

#define MOCK_RETURN(mock, func, value) \
do { \
    mock.func##__.retValues.push(value); \
} while (0)

#define ENSURE_CALL(mock, call) \
do { \
    if (mock.call__##call.calls.empty()) FAIL(#call " not called"); \
    unsigned vi = mock.getCallVerificationIndex(); \
    assert(mock.call__##call.calls.front().callIndex__ >= vi); \
    if (mock.call__##call.calls.front().callIndex__ > vi) FAIL(#call " called too late"); \
    mock.call__##call.calls.front().verify__(__FILE__, __LINE__, mock.call__##call.calls.front()).call; \
    mock.call__##call.calls.pop_front(); \
} while(0)
 
}
}

#endif // RASK_TEST_MOCK_HPP
