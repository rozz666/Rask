// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_GENFUNCTIONIR_HPP
#define RASK_CG_GENFUNCTIONIR_HPP

#include <vector>
#include <rask/ast/Function.hpp>

namespace rask
{
namespace cg
{

typedef std::vector<unsigned char> IRCodeBuffer;
    
IRCodeBuffer genFunctionIR(const ast::Function& f);

}

}

#endif /* RASK_CG_GENFUNCTIONIR_HPP */
