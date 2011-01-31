// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cg/TypeFactory.hpp>
#include <llvm/DerivedTypes.h>

namespace rask
{
namespace cg
{

const llvm::Type* TypeFactory::getType(llvm::LLVMContext& context, rask::ast::BasicType type)
{
    switch (type)
    {
        case ast::INT32: return llvm::IntegerType::get(context, 32);
        case ast::BOOLEAN: return llvm::IntegerType::get(context, 1);
        default: return llvm::Type::getVoidTy(context);
    }
}

}
}



