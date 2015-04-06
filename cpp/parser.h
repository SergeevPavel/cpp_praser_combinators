#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <functional>

template<class A>
class Parser
{
public:

    using output_t = std::vector< std::pair<A, std::string> >; // возвращается вектор, что довольно плохо
    using input_t = std::string;
    using parser_t = std::function< output_t(input_t) >;

    Parser(parser_t const& parser_)
        : parser(parser_)
    {
    }

    output_t apply(input_t input) const
    {
        return parser(input);
    }

private:
    parser_t parser;
};

#endif // PARSER_H

