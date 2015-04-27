#ifndef COMBINATORS_H
#define COMBINATORS_H

#include <vector>
#include <functional>
#include <string>
#include <utility>

#include "parser.h"
#include "function_traits.h"

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

template <class A, class function_t>
auto operator >= (const Parser<A> p, const function_t f) -> typename function_traits<function_t>::result_type
{
    using B = typename function_traits<function_t>::result_type::data_t;
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

template <class A>
Parser<A> operator ||(const Parser<A> p, const Parser<A> q)
{
    return Parser<A>([p, q](typename Parser<A>::input_t input){
        typename Parser<A>::output_t output;
        const typename Parser<A>::output_t p_chunk = p.apply(input);
        const typename Parser<A>::output_t q_chunk = q.apply(input);
        output.insert(output.end(), p_chunk.begin(), p_chunk.end());
        output.insert(output.end(), q_chunk.begin(), q_chunk.end());
        return output;
    });
}

Parser<char> satisfy(const std::function<bool(char)> p)
{
    return item() >= [p](const char x) {
        if (p(x))
        {
            return result(x);
        }
        return zero<char>();
    };
}

template <class A>
Parser< std::vector<A> > many(const Parser<A> p)
{
    return  p       >= [p](const A x)
    {return many(p) >= [x](const std::vector<A> xs){
            std::vector<A> output(xs);
            output.insert(output.begin(), x);
     return result(output);};} || result(std::vector<A>());
}

template <class A>
Parser< std::vector<A> > many1(const Parser<A> p)
{
    return p       >= [p](const A x){
    return many(p) >= [x](const std::vector<A> xs){
           std::vector<A> output(xs);
           output.insert(output.begin(), x);
    return result(output);};};
}



#endif // COMBINATORS_H

