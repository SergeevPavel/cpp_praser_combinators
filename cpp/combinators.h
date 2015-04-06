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
    return Parser<A>([v](typename Parser<A>::input_t input){
        return typename Parser<A>::output_t(1, std::make_pair(v, input));
    });
}

template <class A>
Parser<A> zero()
{
    return Parser<A>([](typename Parser<A>::input_t input){
       return typename Parser<A>::output_t();
    });
}

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

template <class A, class B>
Parser<B> bind(Parser<A> p, std::function< Parser<B>(A) > f)
{
    return Parser<B>([p, f](typename Parser<B>::input_t input){
       typename Parser<B>::output_t output;
       for (auto res : p.apply(input))
       {
           const typename Parser<B>::output_t chunk = f(res.first).apply(res.second);
           output.insert(output.end(), chunk.begin(), chunk.end());
       }
       return output;
    });
}


#endif // COMBINATORS_H

