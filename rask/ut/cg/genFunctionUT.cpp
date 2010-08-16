#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 
#include <rask/ast/Function.hpp>
#include <rask/cg/genFunction.hpp>

namespace tut
{

struct genFunction_TestData
{
};

typedef test_group<genFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.cg.genFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;

    ast::Function f;
    cg::BytecodeBuffer bb = cg::genFunction(f);
    ensure(bb.empty());
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;

    ast::Function f;
    f.addValue(5);
    f.addValue(-2);
    f.addValue(0);

    cg::BytecodeBuffer bb = cg::genFunction(f);

    ensure_equals("size", bb.size(), f.valueCount());
    ensure_equals("elem 1", bb[0], f.value(0));
    ensure_equals("elem 2", bb[1], f.value(1));
    ensure_equals("elem 3", bb[2], f.value(2));
}

}
