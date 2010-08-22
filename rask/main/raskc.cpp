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
#include <rask/ast/parseFunction.hpp>
#include <rask/cg/genFunction.hpp>

struct Parameters
{
    std::vector<std::string> inputFiles;
    std::string outputFile;
    bool noOutput;

    Parameters() : outputFile("a.out"), noOutput(false) { }
};

Parameters parseCommandLine(int argc, char **argv)
{
    namespace po = ::boost::program_options;

    Parameters params;

    po::options_description desc("Options");
    desc.add_options()
        ("output,o", po::value<std::string>(&params.outputFile), "set output file")
        ("no-output,n", "no output file is generated");

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("input-file", po::value<std::vector<std::string> >(&params.inputFiles), "input file");

    po::options_description cmdline_options;
    cmdline_options.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
    po::notify(vm);

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
    rask::InputStream is(params.inputFiles[0], f);
    rask::error::Logger logger;

    boost::optional<rask::cst::Function> mainFunc = rask::cst::parseFile(is, logger);

    if (mainFunc)
    {
        boost::optional<rask::ast::Function> mainFunc2 = rask::ast::parseFunction(*mainFunc, logger);

        if (mainFunc2 && !params.noOutput)
        {
            std::ofstream of(params.outputFile.c_str(), std::ios::binary);

            cg::BytecodeBuffer bb = cg::genFunction(*mainFunc2);
            
            of.write(reinterpret_cast<const char *>(&bb.front()), sizeof(bb[0]) * bb.size());
        }
    }

    std::copy(logger.errors().begin(), logger.errors().end(), std::ostream_iterator<rask::error::Message>(std::cerr, "\n"));
}