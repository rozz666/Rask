#include <rask/cst/Grammar.hpp>
#include <rask/cst/parseFile.hpp>

namespace rask
{
namespace cst
{

boost::optional<Function> parseFile(InputStream& is, error::Logger& logger)
{
    typedef std::vector<char> Source;
    Source source;

    source.assign(std::istreambuf_iterator<char>(is.stream()), std::istreambuf_iterator<char>());

    typedef boost::spirit::classic::position_iterator<Source::const_iterator, boost::spirit::classic::file_position> PosIterator;

    using boost::spirit::ascii::space;
    PosIterator iter(source.begin(), source.end(), is.file());
    PosIterator end;

    typedef rask::cst::Grammar<PosIterator> Grammar;
    Grammar grammar;
    cst::Function function;

    bool r = boost::spirit::qi::phrase_parse(iter, end, grammar, space, function);

    if (r && iter == end)
    {
        return function;
    }
    else
    {
        logger.log(error::Message::missingMainFunction(Position(is.file())));

        return boost::none;
    }
}

}
}