#include <sstream>
#include <rask/InputStream.hpp>
#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 

namespace tut
{

struct InputStream_TestData
{
};

typedef test_group<InputStream_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.InputStream");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    std::stringstream ss;
    rask::InputStream is("xxx", ss);

    ensure_equals("file", is.file(), "xxx");
    ensure_equals("stream", &is.stream(), &ss);
}


}
