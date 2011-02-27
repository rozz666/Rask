// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <iostream>
#include <iterator>
#include <memory>
#include <boost/scoped_ptr.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/system_error.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetData.h>

namespace builtin
{

void rask_print_int(int32_t n)
{
    std::cout << n << std::endl;
}

int32_t rask_get_int_32()
{
    int32_t x;
    if (std::cin >> x) return x;
    else return 0;
}

}

int main(int, char **argv)
{
    llvm::InitializeNativeTarget();

    llvm::OwningPtr<llvm::MemoryBuffer> buf;
    if (llvm::MemoryBuffer::getFile(argv[1], buf))
    {
        std::cerr << "ERROR: MemoryBuffer::getFile failed" << std::endl;
        return 1;
    }

    llvm::LLVMContext context;

    boost::scoped_ptr<llvm::Module> module(llvm::ParseBitcodeFile(buf.get(), context));

    if (!module.get())
    {
        std::cerr << "ERROR: ParseBitcodeFile failed" << std::endl;
        return 1;
    }

    llvm::ExecutionEngine *engine(llvm::EngineBuilder(module.get()).create());

    if (!engine)
    {
        std::cerr << "ERROR: engine creation failed" << std::endl;
        return 1;
    }

    llvm::Function *print = engine->FindFunctionNamed("print");
    engine->addGlobalMapping(print, (void *)(intptr_t)&builtin::rask_print_int);
    llvm::Function *getInt32 = engine->FindFunctionNamed("getInt32");
    engine->addGlobalMapping(getInt32, (void *)(intptr_t)&builtin::rask_get_int_32);

    llvm::Function *mainf = module->getFunction("main");

    if (!mainf)
    {
        std::cerr << "ERROR: void main() not found" << std::endl;
        return 1;
    }

    void ( *mainPtr)() = (void(*)())(intptr_t)engine->getPointerToFunction(mainf);

    if (!mainPtr)
    {
        std::cerr << "ERROR: JIT failed" << std::endl;
    }

    mainPtr();
}