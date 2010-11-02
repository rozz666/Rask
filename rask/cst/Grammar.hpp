// Rask
//
// Copyright (c) 2010 Rafal Przywarski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
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
#include <rask/cst/Tree.hpp>
#include <rask/error/Logger.hpp>

namespace rask
{
namespace cst
{

BOOST_SPIRIT_TERMINAL(inputPos);
BOOST_SPIRIT_TERMINAL_EX(error);

}
}

namespace boost
{
namespace spirit 
{ 
    template <>
    struct use_terminal<qi::domain, rask::cst::tag::inputPos> : mpl::true_  { }; 

    template <typename A0, typename A1>
    struct use_terminal<qi::domain, terminal_ex<rask::cst::tag::error, fusion::vector2<A0, A1> > > : mpl::true_ { };

    template <>                    
    struct use_lazy_terminal<qi::domain, rask::cst::tag::error, 2> : mpl::true_ { };
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
        boost::spirit::traits::assign_to(Position(first.get_position().file, first.get_position().line, first.get_position().column), attr);
        return true;
    }

    template <typename Context>
    boost::spirit::info what(Context&) const
    {
        return boost::spirit::info("inputPos");
    }
};

struct errorParser : boost::spirit::qi::primitive_parser<errorParser>
{
    template <typename Context, typename Iterator>
    struct attribute
    {
        typedef boost::spirit::unused_type type;
    };

    typedef error::Message ( *Factory)(const Position& );
    Factory messageFactory;
    error::Logger *logger;

    errorParser(Factory messageFactory, error::Logger& logger) : messageFactory(messageFactory), logger(&logger) { }

    template <typename Iterator, typename Context
        , typename Skipper, typename Attribute>
    bool parse(Iterator& first, Iterator const& last, Context& , Skipper const& skipper, Attribute& ) const
    {
        boost::spirit::qi::skip_over(first, last, skipper);
        logger->log(messageFactory(Position(first.get_position().file, first.get_position().line, first.get_position().column)));
        return true;
    }

    template <typename Context>
    boost::spirit::info what(Context& ) const
    {
        return boost::spirit::info("errorParser");
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

template <typename Modifiers, typename A0, typename A1>
struct make_primitive<terminal_ex<rask::cst::tag::error, fusion::vector2<A0, A1> >, Modifiers>
{
    typedef rask::cst::errorParser result_type;

    template <typename Terminal>
    result_type operator()(Terminal const& term, unused_type) const
    {
        return result_type(fusion::at_c<0>(term.args), *fusion::at_c<1>(term.args));
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

namespace detail
{

struct errorMessageImpl
{
    template <typename Logger, typename What, typename It>
    struct result { typedef void type; };

    template <typename Logger, typename What, typename It>
    void operator()(Logger *logger, What what, It it) const
    {
        if (what.value.type() == typeid(std::string))
        {
            std::string val = boost::get<std::string>(what.value);
            Position pos(it.get_position().file, it.get_position().line, it.get_position().column);

            if (val == "{")
            {
                logger->log(error::Message::missingOpeningBrace(pos));
            }
            else if (val == "}")
            {
                logger->log(error::Message::missingClosingBrace(pos));
            }
            else if (val == "(")
            {
                logger->log(error::Message::missingOpeningParen(pos));
            }
            else if (val == ")")
            {
                logger->log(error::Message::missingClosingParen(pos));
            }
            else if (val == "->")
            {
                logger->log(error::Message::missingRightArrow(pos));
            }
            else if (val == ";")
            {
                logger->log(error::Message::missingSemicolon(pos));
            }
        }
    }
};

boost::phoenix::function<errorMessageImpl> errorMessage; 

}

template <typename Iterator>
struct Grammar : qi::grammar<Iterator, cst::Tree(), ascii::space_type>
{
    Grammar(error::Logger& errorLogger)
        : Grammar::base_type(tree, "tree")
    {
        identifier %= inputPos >> qi::lexeme[qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z0-9_")];
        constant %= inputPos >> qi::int_;
        expression %= constant | identifier;
        functionCall %= identifier > '(' > -(expression % ',') > ')';
        variableDeclaration %= qi::lit("var") > identifier > -('=' > expression);
        statement %= (variableDeclaration | functionCall) > ';';
        function %=
            identifier > '(' > -((qi::lit("int32") > identifier) % ',') > ')' > "->" > (identifier | error(&error::Message::missingReturnType, &errorLogger)) >
            '{' > *statement > '}';
        tree %= *function > inputPos > qi::eoi;

        qi::on_error<qi::fail>
        (
            tree,
            detail::errorMessage(&errorLogger, qi::labels::_4, qi::labels::_3)
        );
    }

    qi::rule<Iterator, cst::Tree(), ascii::space_type> tree;
    qi::rule<Iterator, cst::Identifier(), ascii::space_type> identifier;
    qi::rule<Iterator, cst::Function(), ascii::space_type> function;
    qi::rule<Iterator, cst::FunctionCall(), ascii::space_type> functionCall;
    qi::rule<Iterator, cst::Statement(), ascii::space_type> statement;
    qi::rule<Iterator, cst::VariableDecl(), ascii::space_type> variableDeclaration;
    qi::rule<Iterator, cst::Constant(), ascii::space_type> constant;
    qi::rule<Iterator, cst::Expression(), ascii::space_type> expression;
};

}
}

#endif /* RASK_CST_GRAMMAR_HPP */

