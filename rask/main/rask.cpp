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
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Target/TargetData.h>

namespace builtin
{

void _rask_print_int(int32_t n)
{   
    std::cout << n << std::endl;
}

}

int main(int, char **argv)
{
    llvm::InitializeNativeTarget();
    
    std::auto_ptr<llvm::MemoryBuffer> buf(llvm::MemoryBuffer::getFile(argv[1]));

    if (!buf.get())
    {
        std::cerr << "ERROR: MemoryBuffer::getFile failed" << std::endl;
        return 1;
    }
    
    llvm::LLVMContext& context = llvm::getGlobalContext();
    
    llvm::Module *module = llvm::ParseBitcodeFile(buf.release(), context);

    if (!module)
    {
        std::cerr << "ERROR: ParseBitcodeFile failed" << std::endl;
        return 1;
    }

    llvm::ExecutionEngine *engine = llvm::EngineBuilder(module).create();

    if (!engine)
    {
        std::cerr << "ERROR: engine creation failed" << std::endl;
        return 1;
    }

    llvm::Function *print = engine->FindFunctionNamed("_rask_print_int");
    engine->addGlobalMapping(print, (void *)(intptr_t)&builtin::_rask_print_int);

   
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