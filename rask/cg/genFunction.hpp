#ifndef RASK_CG_GENFUNCTION_HPP
#define RASK_CG_GENFUNCTION_HPP

#include <vector>
#include <rask/ast/Function.hpp>

namespace rask
{
namespace cg
{

typedef std::vector<int> BytecodeBuffer;

BytecodeBuffer genFunction(const ast::Function& f);

}
}
#endif /* RASK_CG_GENFUNCTION_HPP */
