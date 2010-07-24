#include <iostream>
#include <iterator>
#include <fstream>
#include <rask/cst/parseFile.hpp>

int main(int, char **argv)
{
    std::ifstream f(argv[1]);
    rask::InputStream is(argv[1], f);
    rask::error::Logger logger;

    boost::optional<rask::cst::Function> mainFunc = rask::cst::parseFile(is, logger);

    std::copy(logger.errors().begin(), logger.errors().end(), std::ostream_iterator<rask::error::Message>(std::cerr, "\n"));
}