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
#include <rask/cst/Parser.hpp>
#include <rask/ast/Builder.hpp>
#include <rask/ast/BuiltinFunctions.hpp>
#include <rask/cg/CodeGenerator.hpp>
#include <llvm/LLVMContext.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <di/injector.hpp>
#include <boost/concept_check.hpp>

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

struct CompilerModule
{
    rask::ast::BuiltinFunctions builtinFunctions;

    void operator()(di::registry& r)
    {
        using namespace rask;

        std::auto_ptr<ast::FunctionTable> ft(new ast::FunctionTable);
        builtinFunctions.declare(*ft);

        r.add(
            r.type<cst::Parser>()
        );
        r.add(
            r.type<ast::Builder>()
            .in_scope<di::scopes::singleton>()
        );
        r.add(
            r.type<error::Logger>()
            .in_scope<di::scopes::singleton>()
        );
        r.add(
            r.type<ast::FunctionTable>()
            .instance(ft.release())
        );
        r.add(
            r.type<cg::CodeGenerator>()
        );
    }
};

int main(int argc, char **argv)
{
    using namespace rask;

    try
    {

        Parameters params = parseCommandLine(argc, argv);

        if (params.inputFiles.empty())
        {
            std::cerr << "raskc: no input" << std::endl;
            return 1;
        }

        std::ifstream f(params.inputFiles[0].c_str());
        InputStream is(params.inputFiles[0], f);

        di::injector injector;
        CompilerModule compilerModule;
        injector.install(compilerModule);

        cst::SharedParser parser = injector.construct<cst::SharedParser>();
        boost::optional<cst::Tree> cst = parser->parseFile(is);

        if (cst)
        {
            ast::SharedBuilder builder = injector.construct<ast::SharedBuilder>();

            ast::SharedScopeFactory scopeFactory(new ast::ScopeFactory);
            boost::optional<ast::Tree> ast = builder->buildTree(*cst, scopeFactory);

            if (ast && !params.noOutput)
            {
                llvm::LLVMContext context;
                llvm::Module module("mainModule", context);
                cg::SharedCodeGenerator cg = injector.construct<cg::SharedCodeGenerator>();

                cg->genModule(*ast, context, module);

                std::vector<unsigned char> buf;
                llvm::BitstreamWriter bw(buf);
                llvm::WriteBitcodeToStream(&module, bw);

                std::ofstream of(params.outputFile.c_str(), std::ios::binary);

                of.write(reinterpret_cast<const char *>(&buf.front()), buf.size());
            }
        }

        error::Logger& logger = injector.construct<error::Logger& >();
        std::copy(logger.errors().begin(), logger.errors().end(), std::ostream_iterator<error::Message>(std::cerr, "\n"));
    }
    catch (const std::exception& e)
    {
        std::cerr << "raskc: error: " << e.what() << std::endl;
    }
}