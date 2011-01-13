// Rask
//
// Copyright (c) 2010-2011 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <rask/cst/Parser.hpp>
#include <rask/cst/Grammar.hpp>

namespace rask
{
namespace cst
{

boost::optional<Tree> Parser::parseFile(InputStream& is)
{
    typedef
        boost::spirit::classic::position_iterator<InputStream::const_iterator, boost::spirit::classic::file_position>
        PosIterator;

    PosIterator iter(is.begin(), is.end(), is.file());
    PosIterator end;

    rask::cst::Grammar<PosIterator> grammar(logger_);
    cst::Tree cst;

    bool r = boost::spirit::qi::phrase_parse(iter, end, grammar, boost::spirit::ascii::space, cst);

    if (r && iter == end && logger_.errors().empty() && cst.functions.size() > 0)
    {
        return cst;
    }
    else
    {
        if (iter == end && logger_.errors().empty())
        {
            logger_.log(error::Message::missingMainFunction(Position(is.file())));
        }

        return boost::none;
    }
}

}
}
