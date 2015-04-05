#ifndef COMBINATORS_H
#define COMBINATORS_H

#include <vector>
#include <functional>
#include <string>
#include <utility>

#include "parser.h"

template <class A>
Parser<A> result(A const v)
{
    return Parser<A>([&v](typename Parser<A>::parser_input_t input){
        return typename Parser<A>::parser_output_t(1, std::make_pair(v, input));
    });
}

template <class A>
Parser<A> zero()
{
    return Parser<A>([](typename Parser<A>::parser_input_t input){
       return typename Parser<A>::parser_output_t();
    });
}

Parser<char> item()
{
    return Parser<char>([](typename Parser<char>::parser_input_t input)
    {
       if (input.length() == 0)
       {
           return Parser<char>::parser_output_t();
       }
       else
       {
           return Parser<char>::parser_output_t(1, std::make_pair(input[0], input.substr(1, input.length() - 1)));
       }
    });
}

#endif // COMBINATORS_H

