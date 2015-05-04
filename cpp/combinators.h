#ifndef COMBINATORS_H
#define COMBINATORS_H

#include <list>
#include <functional>
#include <string>
#include <utility>

#include "parser.h"
#include "function_traits.h"

template <class A>
Parser< std::list<A> > many(Parser<A> const& p)
{
    return  p       >= [p](const A x)
    {return many(p) >= [x](std::list<A> xs){
     xs.push_front(x);
     return result(xs);};} || result(std::list<A>());
}

template <class A>
Parser< std::list<A> > many1(Parser<A> const& p)
{
    return p       >= [p](const A x){
    return many(p) >= [x](std::list<A> xs){
    xs.push_front(x);
    return result(xs);};};
}



#endif // COMBINATORS_H

