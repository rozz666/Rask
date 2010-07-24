#ifndef RASK_AST_FUNCTION_HPP
#define RASK_AST_FUNCTION_HPP

#include <vector>

namespace rask
{
namespace ast
{

class Function
{
public:

    std::vector<int> values;

    std::size_t valueCount() const { return values.size(); }
    int value(std::size_t idx) const { return values[idx]; }
    void addValue(int value) { values.push_back(value); }
};

}
}

#endif /* RASK_AST_FUNCTION_HPP */