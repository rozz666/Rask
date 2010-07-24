#include <tut/tut.hpp>
#include <tut/../contrib/tut_macros.h> 
#include <rask/ast/parseFunction.hpp>

namespace tut
{

struct parseFunction_TestData
{
    rask::error::Logger logger;
    const std::string file;
    rask::cst::Function cf;
    
    parseFunction_TestData() : file("test.rask")
    {
        cf.name.value = "main";
        cf.name.position = rask::Position(file, 1, 2);
    }
};

typedef test_group<parseFunction_TestData> factory;
typedef factory::object object;
}

namespace
{
tut::factory tf("rask.ast.parseFunction");
}

namespace tut
{

template <>
template <>
void object::test<1>()
{
    using namespace rask;
    
    cf.calls.resize(2);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);
    cf.calls[0].args.push_back(1);

    cf.calls[1].function.value = "print";
    cf.calls[1].function.position = Position(file, 3, 4);
    cf.calls[1].args.push_back(2);

    boost::optional<ast::Function> af = ast::parseFunction(cf, logger);

    ensure("parsed", af.is_initialized());
    ensure_equals("no errors", logger.errors().size(), 0u);
    ensure_equals("value count", af->valueCount(), 2u);
    ensure_equals("value 1", af->value(0), 1);
    ensure_equals("value 2", af->value(1), 2);
}

template <>
template <>
void object::test<2>()
{
    using namespace rask;
    
    cf.calls.resize(1);
    cf.calls[0].function.value = "xxx";
    cf.calls[0].function.position = Position(file, 2, 4);

    boost::optional<ast::Function> af = ast::parseFunction(cf, logger);

    ensure_not("not parsed", af.is_initialized());
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::unknownIdentifier(cf.calls[0].function.position, cf.calls[0].function.value));
}

template <>
template <>
void object::test<3>()
{
    using namespace rask;
   
    cf.calls.resize(1);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);

    boost::optional<ast::Function> af = ast::parseFunction(cf, logger);

    ensure_not("not parsed", af.is_initialized());
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(cf.calls[0].function.position, "print()"));
}

template <>
template <>
void object::test<4>()
{
    using namespace rask;
    
    cf.calls.resize(1);
    cf.calls[0].function.value = "print";
    cf.calls[0].function.position = Position(file, 2, 4);
    cf.calls[0].args.push_back(1);
    cf.calls[0].args.push_back(2);

    boost::optional<ast::Function> af = ast::parseFunction(cf, logger);

    ensure_not("not parsed", af.is_initialized());
    ensure_equals("errors", logger.errors().size(), 1u);
    ensure_equals(logger.errors()[0], error::Message::functionNotFound(cf.calls[0].function.position, "print(int, int)"));
}

}
