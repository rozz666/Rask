// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef RASK_CG_TYPEFACTORY_HPP
#define RASK_CG_TYPEFACTORY_HPP

#include <boost/shared_ptr.hpp>
#include <llvm/Type.h>
#include <rask/ast/BasicType.hpp>

namespace rask
{
namespace cg
{

class TypeFactory
{
public:

    virtual ~TypeFactory() { }
    virtual const llvm::Type *getType(llvm::LLVMContext& context, ast::BasicType type);
};

typedef boost::shared_ptr<TypeFactory> SharedTypeFactory;

}
}

#endif // RASK_CG_TYPEFACTORY_HPP
