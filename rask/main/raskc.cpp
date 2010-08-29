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
#include <fstream>
#include <boost/program_options.hpp> 
#include <rask/cst/parseFile.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Bitcode/ReaderWriter.h>

struct Parameters
{
    std::vector<std::string> inputFiles;
    std::string outputFile;
    bool noOutput;

    Parameters() : outputFile("a.out"), noOutput(false) { }
};

Parameters parseCommandLine(int argc, char **argv)
{
    using namespace boost::program_options;

    Parameters params;

    options_description desc("Options");
    desc.add_options()
        ("output,o", value<std::string>(&params.outputFile), "set output file")
        ("no-output,n", "no output file is generated");

    options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", value<std::vector<std::string> >(&params.inputFiles), "input file");

    options_description cmdline_options;
    cmdline_options.add(desc).add(hidden);

    positional_options_description p;
    p.add("input-file", -1);

    variables_map vm;
    store(command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
    notify(vm);

    params.noOutput = vm.count("no-output") != 0;

    return params;
}

int main(int argc, char **argv)
{
    using namespace rask;

    Parameters params = parseCommandLine(argc, argv);
   
    if (params.inputFiles.empty())
    {
        std::cerr << "raskc: no input" << std::endl;
        return 1;
    } 

    std::ifstream f(params.inputFiles[0].c_str());
    InputStream is(params.inputFiles[0], f);
    error::Logger logger;

    boost::optional<rask::cst::Tree> cst = rask::cst::parseFile(is, logger);

    if (cst)
    {
        boost::optional<rask::ast::Tree> ast = ast::Builder().buildTree(*cst, logger);

        if (ast && !params.noOutput)
        {
            llvm::LLVMContext context;
            cg::CodeGenerator cg;

            std::auto_ptr<llvm::Module> module = cg.genModule(*ast, context);

            std::vector<unsigned char> buf;
            llvm::BitstreamWriter bw(buf);
            llvm::WriteBitcodeToStream(module.get(), bw);

            std::ofstream of(params.outputFile.c_str(), std::ios::binary);

            of.write(reinterpret_cast<const char *>(&buf.front()), buf.size());
        }
    }

    std::copy(logger.errors().begin(), logger.errors().end(), std::ostream_iterator<rask::error::Message>(std::cerr, "\n"));
}