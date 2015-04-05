#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <functional>

template<class A>
class Parser
{
public:

    using parser_output_t = std::vector< std::pair<A, std::string> >; // возвращается вектор, что довольно плохо
    using parser_input_t = std::string;
    using parser_t = std::function< parser_output_t(parser_input_t) >;

    Parser(parser_t const& parser_)
        : parser(parser_)
    {
    }

    parser_output_t apply(parser_input_t input)
    {
        return parser(input);
    }

private:
    parser_t parser;
};

#endif // PARSER_H

