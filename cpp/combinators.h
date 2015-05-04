#ifndef COMBINATORS_H
#define COMBINATORS_H

#include <vector>
#include <functional>
#include <string>
#include <utility>

#include "parser.h"
#include "function_traits.h"



template <class A>
Parser< std::vector<A> > many(Parser<A> const& p)
{
    return  p       >= [p](const A x)
    {return many(p) >= [x](const std::vector<A>& xs){
            std::vector<A> output(xs);
            output.insert(output.begin(), x);
     return result(output);};} || result(std::vector<A>());
}

template <class A>
Parser< std::vector<A> > many1(Parser<A> const& p)
{
    return p       >= [p](const A x){
    return many(p) >= [x](const std::vector<A>& xs){
           std::vector<A> output(xs);
           output.insert(output.begin(), x);
    return result(output);};};
}



#endif // COMBINATORS_H

