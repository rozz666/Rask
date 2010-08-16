#include <rask/cg/genFunction.hpp>

namespace rask
{
namespace cg
{

BytecodeBuffer genFunction(const ast::Function& f)
{
    BytecodeBuffer bb;
    bb.reserve(f.valueCount());
    
    for (std::size_t i = 0; i != f.valueCount(); ++i)
    {
        bb.push_back(f.value(i));
    }

    return bb;
}

}
}
