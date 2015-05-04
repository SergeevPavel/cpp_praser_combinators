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
    {return many(p) >= [x](std::list<A> xs){ // TODO perf try to add reference
     xs.push_front(x);
     return result(xs);};} || result(std::list<A>());
}

template <class A>
Parser< std::list<A> > many1(Parser<A> const& p)
{
    return p       >= [p](const A x){
    return many(p) >= [x](std::list<A> xs){ // TODO perf try to add reference
    xs.push_front(x);
    return result(xs);};};
}

template <class A, class B>
Parser< std::list<A> > sepby1(Parser<A> const& p, Parser<B> const& sep) {
    const Parser<A> chunk = (sep >= [p](const B _){
                    return   p   >= [](const A x){
                    return   result(x);};});
    return p           >= [chunk](const A x){
    return many(chunk) >= [x](std::list<A> xs){
    xs.push_front(x);
    return result(xs);};};
}

template <class A, class B>
Parser< std::list<A> > sepby(Parser<A> const p, Parser<B> const& sep) {
    return sepby1(p, sep) || result(std::list<A>());
}

#endif // COMBINATORS_H

