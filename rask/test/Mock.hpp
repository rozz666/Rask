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
#include <deque>
#include <map>
#include <boost/preprocessor.hpp>
#include <boost/assert.hpp>
#include <rask/test/TUTAssert.hpp>

namespace rask
{
namespace test
{

class MockBase
{
public:

    MockBase() : ci(0), vi(0) { }

    unsigned getCallIndex() const { return ++ci; }
    unsigned getCallVerificationIndex() const { return ++vi; }

private:

    mutable unsigned ci;
    mutable unsigned vi;
};

template <typename T>
struct StorageType
{
    typedef T type;
    static type convert(T v) { return v; }
};

template <typename T>
struct StorageType<T&>
{
    typedef T *type;
    static type convert(T& v) { return &v; }
    static T& convert(type v) { return *v; }
};

template <typename T>
struct Storage
{
    typename StorageType<T>::type value;
    Storage() { }
    Storage(T v) : value(StorageType<T>::convert(v)) { }
    T get() { return StorageType<T>::convert(value); }
    void set(T v) { value = StorageType<T>::convert(v); }
};

template <typename T>
struct StorageQueue
{
    std::deque<Storage<T> > values;

    void push_back(T v) { values.push_back(v); }
    T get(std::string name)
    {
        if (values.empty()) tut::fail("No return value specified for " + name);
        Storage<T> val = values.front();
        values.pop_front();
        return val.get();
    }
};

#define MOCK(name, parent) struct name : parent, ::rask::test::MockBase

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
#define MOCK_ENUM_ARG(r, data, i, elem) BOOST_PP_COMMA_IF(i) BOOST_PP_TUPLE_ELEM(2, 1, elem)
#define MOCK_TEST_ARG(r, data, i, elem) \
if (!Test<BOOST_PP_TUPLE_ELEM(2, 0, elem)>::equal(BOOST_PP_TUPLE_ELEM(2, 1, elem), call.BOOST_PP_TUPLE_ELEM(2, 1, elem))) \
{ \
    std::ostringstream ss; \
    ss << "Mismatched parameter " << (i + 1); \
    tut::fail(tut::formatMessage(ss.str().c_str(), file, line)); \
}

#define MOCK_CHECK_LESS_ARG(r, data, i, elem) \
if (Test<BOOST_PP_TUPLE_ELEM(2, 0, elem)>::less(BOOST_PP_TUPLE_ELEM(2, 1, elem), data.BOOST_PP_TUPLE_ELEM(2, 1, elem))) \
{ \
    return true; \
} \
if (Test<BOOST_PP_TUPLE_ELEM(2, 0, elem)>::less(data.BOOST_PP_TUPLE_ELEM(2, 1, elem), BOOST_PP_TUPLE_ELEM(2, 1, elem))) \
{ \
    return false; \
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
    BOOST_PP_SEQ_FOR_EACH_I(MOCK_ENUM_ARG, _, args)

#define MOCK_TEST_ARGS(args) \
    BOOST_PP_SEQ_FOR_EACH_I(MOCK_TEST_ARG, _, args)

#define MOCK_CHECK_LESS_ARGS(args, right) \
    BOOST_PP_SEQ_FOR_EACH_I(MOCK_CHECK_LESS_ARG, right, args) \
    return false;

#define MOCK_METHOD_Q(retType, name, altName, args, qualifiers) \
struct name##__type { \
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
            template <typename T> \
            struct Test { \
                static bool equal(const T& left, const T& right) { return left == right; } \
            }; \
            \
            template <typename T> \
            struct Test<T&> { \
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
    mutable boost::ptr_deque<Call> calls; \
} mutable name##__; \
template <typename RetType, bool dummy = true> \
struct ReturnMap__##name \
{ \
    struct Key \
    { \
        MOCK_DECL_MEMBERS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) \
        template <typename T, bool ref = boost::is_reference<T>::value> \
        struct Test { \
            static bool less(const T& left, const T& right) { return std::less<T>()(left, right); } \
        }; \
        \
        template <typename T> \
        struct Test<T, true> { \
            static bool less(T left, T right) \
            { \
                return std::less<typename boost::remove_reference<T>::type *>()(&left, &right); \
            } \
        }; \
        bool operator<(const Key& right) const \
        { \
            MOCK_CHECK_LESS_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END), right) \
        } \
    }; \
    typedef ::rask::test::StorageQueue<RetType> ValueQueue; \
    typedef std::map<Key, ValueQueue> Values; \
    Values values; \
    ValueQueue simpleValues; \
     \
    ValueQueue& operator()(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) \
    { \
        Key key = { MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) };\
        return values[key];\
    } \
    bool isMapped(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) \
    { \
        Key key = { MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) };\
        return values.find(key) != values.end(); \
    } \
    RetType get(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) \
    { \
        Key key = { MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) };\
        return values.find(key)->second.get(#name); \
    } \
    void push_back(RetType val) \
    { \
        simpleValues.push_back(val);\
    }\
    RetType getSimple() \
    { \
        return simpleValues.get(#name); \
    } \
}; \
template <bool dummy> \
struct ReturnMap__##name<void, dummy> \
{ \
    void operator()(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) { }\
    bool isMapped(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) { return false; }\
    void get(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) { } \
    void getSimple() { } \
}; \
mutable ReturnMap__##name<retType> map__##name; \
name##__type& call__##name(MOCK_DECL_ARG_TYPES(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) qualifiers \
{ \
    return name##__; \
} \
retType altName(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) qualifiers \
{ \
    if (map__##name.isMapped(MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)))) \
    { \
        return map__##name.get(MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))); \
    } \
    name##__type::Call c = { getCallIndex(), MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END)) }; \
    name##__.calls.push_back(new name##__type::Call(c)); \
    return map__##name.getSimple(); \
}

#define MOCK_METHOD(retType, name, args) \
    MOCK_METHOD_Q(retType, name, name, args, )

#define MOCK_CONST_METHOD(retType, name, args) \
    MOCK_METHOD_Q(retType, name, name, args, const)

#define MOCK_METHOD_TRANSFORM_Q(retType, name, args, newArgs, qualifiers) \
MOCK_METHOD_Q(retType, name, transformed__##name, newArgs, qualifiers) \
retType name(MOCK_DECL_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 args,_END))) qualifiers \
{ \
    return transformed__##name(MOCK_ENUM_ARGS(BOOST_PP_CAT(MOCK_METHOD_FILLER_0 newArgs,_END))); \
}

#define MOCK_METHOD_TRANSFORM(retType, name, args, newArgs) \
    MOCK_METHOD_TRANSFORM_Q(retType, name, args, newArgs, )

#define MOCK_CONST_METHOD_TRANSFORM(retType, name, args, newArgs) \
    MOCK_METHOD_TRANSFORM_Q(retType, name, args, newArgs, const)

#define MOCK_RETURN(mock, call, value) \
do { \
    (mock).map__##call.push_back(value); \
} while (0)

#define ENSURE_CALL(mock, call) \
do { \
    if ((mock).call__##call.calls.empty()) FAIL(#call " not called"); \
    unsigned vi = (mock).getCallVerificationIndex(); \
    BOOST_ASSERT((mock).call__##call.calls.front().callIndex__ >= vi); \
    if ((mock).call__##call.calls.front().callIndex__ > vi) FAIL(#call " called too late"); \
    (mock).call__##call.calls.front().verify__(__FILE__, __LINE__, (mock).call__##call.calls.front()).call; \
    (mock).call__##call.calls.pop_front(); \
} while(0)

#define ENSURE_NO_CALLS(mock, func) \
do { \
    if (!(mock).func##__.calls.empty()) FAIL(#func " called"); \
} while (0)

}
}

#endif // RASK_TEST_MOCK_HPP
