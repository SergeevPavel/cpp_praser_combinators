#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <string>
#include <functional>
#include <memory>

#include "function_traits.h"

template<class A>
class Parser
{
public:
    using data_t = A;
    using output_t = std::list< std::pair<A, std::string> >;
    using input_t = std::string;
    using parser_t = std::function< output_t(input_t) >;


    output_t apply(input_t input) const
    {
        return (*parser_)(input);
    }

    void copy_context(Parser const& other)
    {
        *parser_ = *other.parser_;
    }

private:
    // monad
    template <class T>
    friend Parser<T> result(T const v); // return

    template <class T, class function_t>
    friend auto operator >= (Parser<T> const& p, const function_t f) -> typename function_traits<function_t>::result_type; // bind

    // alternative
    template <class T>
    friend Parser<T> zero(); // mzero

    template <class T>
    friend Parser<T> operator || (Parser<T> const& p, Parser<T> const& q); // mplus

    // simple parsers
    friend Parser<char> item(); // get one char

    Parser(parser_t const& parser)
        : parser_(std::make_shared<parser_t>(parser))
    {

    }

    std::shared_ptr<parser_t> parser_;
};

// Monadic operators

template <class A>
Parser<A> result(A const v)
{
    return Parser<A>([v](typename Parser<A>::input_t input){
        return typename Parser<A>::output_t(1, std::make_pair(v, input));
    });
}

template <class A, class function_t>
auto operator >= (Parser<A> const& p, const function_t f) -> typename function_traits<function_t>::result_type
{
    using B = typename function_traits<function_t>::result_type::data_t;
    return Parser<B>([p, f](typename Parser<B>::input_t input){
       typename Parser<B>::output_t output;
       for (auto res : p.apply(input))
       {
           output.splice(output.end(), f(res.first).apply(res.second));
       }
       return output;
    });
}

// end of monadic operators

// Alternative operators

template <class A>
Parser<A> zero()
{
    return Parser<A>([](typename Parser<A>::input_t input){
       return typename Parser<A>::output_t();
    });
}

template <class A>
Parser<A> operator || (Parser<A> const& p, Parser<A> const& q)
{
    return Parser<A>([p, q](typename Parser<A>::input_t input){
        typename Parser<A>::output_t output = p.apply(input);
        output.splice(output.end(), q.apply(input));
        return output;
    });
}

// end of alternative operators

Parser<char> item()
{
    return Parser<char>([](typename Parser<char>::input_t input)
    {
       if (input.length() == 0)
       {
           return Parser<char>::output_t();
       }
       else
       {
           return Parser<char>::output_t(1, std::make_pair(input[0], input.substr(1, input.length() - 1)));
       }
    });
}

Parser<char> satisfy(std::function<bool(char)> const& p)
{
    return item() >= [p](const char x) {
        if (p(x))
        {
            return result(x);
        }
        return zero<char>();
    };
}

#endif // PARSER_H

