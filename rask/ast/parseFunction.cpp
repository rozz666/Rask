#include <sstream>
#include <boost/foreach.hpp>
#include <rask/ast/parseFunction.hpp>

namespace rask
{
namespace ast
{

boost::optional<Function> parseFunction(const cst::Function& cf, error::Logger& logger)
{
    Function af;

    BOOST_FOREACH(const cst::FunctionCall& call, cf.calls)
    {
        if (call.function.value != "print")
        {
            logger.log(error::Message::unknownIdentifier(call.function.position, call.function.value));
            return boost::none;
        }

        if (call.args.size() != 1)
        {
            std::ostringstream os;

            os << "print(";

            if (!call.args.empty())
            {
                os << "int";

                for (auto n = call.args.size(); n != 1; --n) os << ", int";
            }

            os << ")";

            logger.log(error::Message::functionNotFound(call.function.position, os.str()));
            return boost::none;
        }

        af.addValue(call.args[0]);
    }

    return af;
}

}
}