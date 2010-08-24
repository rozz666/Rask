// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cst/Grammar.hpp>
#include <rask/cst/parseFile.hpp>

namespace rask
{
namespace cst
{

boost::optional<Tree> parseFile(InputStream& is, error::Logger& logger)
{
    typedef std::vector<char> Source;
    Source source;

    source.assign(std::istreambuf_iterator<char>(is.stream()), std::istreambuf_iterator<char>());

    typedef boost::spirit::classic::position_iterator<Source::const_iterator, boost::spirit::classic::file_position> PosIterator;

    using boost::spirit::ascii::space;
    PosIterator iter(source.begin(), source.end(), is.file());
    PosIterator end;

    typedef rask::cst::Grammar<PosIterator> Grammar;
    Grammar grammar(logger);
    cst::Tree cst;

    bool r = boost::spirit::qi::phrase_parse(iter, end, grammar, space, cst.main);

    if (r && iter == end && logger.errors().empty())
    {
        return cst;
    }
    else
    {
        if (iter == end && logger.errors().empty())
        {
            logger.log(error::Message::missingMainFunction(Position(is.file())));
        }

        return boost::none;
    }
}

}
}