#ifndef COMBINATORS_H
#define COMBINATORS_H

#include <vector>
#include <functional>
#include <string>
#include <utility>

//#include <boost/type_traits/function_traits.hpp>
// iserted code
template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};
// For generic types, directly use the result of the signature of its 'operator()'

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
// we specialize for pointers to member function
{
    enum { arity = sizeof...(Args) };
    // arity is the number of arguments.

    typedef ReturnType result_type;

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
        // the i-th argument is equivalent to the i-th tuple element of a tuple
        // composed of those arguments.
    };
};
// end of inserted code

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

template <class A, class function_t>
auto bind(const Parser<A> p, const function_t f) //-> typename decltype(std::function(f))//function_t::result_type
{
    using B = typename function_traits<decltype(f)>::result_type::data_t;
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
Parser<A> plus(const Parser<A> p, const Parser<A> q)
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
    return bind(item(), [p](const char x) {
        if (p(x))
        {
            return result(x);
        }
        return zero<char>();
    });
}

template <class A>
Parser< std::vector<A> > many(const Parser<A> p)
{
    const Parser<std::vector<A> > chunk = bind(p, [p](const A x){
                                   return bind(many(p), [x](const std::vector<A> xs){
                                   std::vector<A> output(xs);
                                   output.insert(output.begin(), x);
                                   return result(output);
        });
    });
    return plus< std::vector<A> >(chunk, result(std::vector<A>()));
}

template <class A>
Parser< std::vector<A> > many1(const Parser<A> p)
{
    return bind(p, [p](const A x){
    return bind(many(p), [x](const std::vector<A> xs){
           std::vector<A> output(xs);
           output.insert(output.begin(), x);
           return result(output);
        });
    });
}



#endif // COMBINATORS_H

