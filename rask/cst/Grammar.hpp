#ifndef RASK_CST_GRAMMAR_HPP
#define RASK_CST_GRAMMAR_HPP

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/tuple/tuple.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/home/classic/iterator/position_iterator.hpp>

#include <rask/Position.hpp>
#include <rask/cst/Function.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace cst
{
    BOOST_SPIRIT_TERMINAL(inputPos);
}
}

namespace boost
{
namespace spirit 
{ 
    template <>
    struct use_terminal<qi::domain, rask::cst::tag::inputPos> : mpl::true_  { }; 
}
}

namespace rask
{
namespace cst
{
struct inputPosParser : boost::spirit::qi::primitive_parser<inputPosParser>
{
    template <typename Context, typename Iterator>
    struct attribute
    {
        typedef rask::Position type;
    };

    template <typename Iterator, typename Context, typename Skipper, typename Attribute>
    bool parse(Iterator& first, Iterator const& last, Context&, Skipper const& skipper, Attribute& attr) const
    {
        boost::spirit::qi::skip_over(first, last, skipper);
        boost::spirit::traits::assign_to(rask::Position(first.get_position().file, first.get_position().line, first.get_position().column), attr);
        return true;
    }

    template <typename Context>
    boost::spirit::info what(Context&) const
    {
        return boost::spirit::info("inputPos");
    }
};
}
}

namespace boost
{
namespace spirit
{
namespace qi
{
template <typename Modifiers>
struct make_primitive<rask::cst::tag::inputPos, Modifiers>
{
    typedef rask::cst::inputPosParser result_type;

    result_type operator()(unused_type, unused_type) const
    {
        return result_type();
    }
};
}
}
}

namespace rask
{
namespace cst
{

namespace qi = ::boost::spirit::qi;
namespace ascii = ::boost::spirit::ascii;

template <typename Iterator>
struct Grammar : qi::grammar<Iterator, cst::Function(), ascii::space_type> 
{
    Grammar() : Grammar::base_type(function, "function")
    {
        identifier %= inputPos >> qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z0-9_")];
        function %= identifier >> '(' >> ')' >> "->" >> "void" >> '{' >> '}' >> qi::eoi;

        identifier.name("identifier");
        function.name("function definition");
    }

    qi::rule<Iterator, cst::Identifier(), ascii::space_type> identifier;
    qi::rule<Iterator, cst::Function(), ascii::space_type> function;
};

}
}

#endif /* RASK_CST_GRAMMAR_HPP */