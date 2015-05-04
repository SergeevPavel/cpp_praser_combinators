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
    const Parser<A> chunk =  sep >= [p](const B _){
                    return   p   >= [](const A x){
                    return   result(x);};};
    return p           >= [chunk](const A x){
    return many(chunk) >= [x](std::list<A> xs){
    xs.push_front(x);
    return result(xs);};};
}

template <class A, class B>
Parser< std::list<A> > sepby(Parser<A> const& p, Parser<B> const& sep) {
    return sepby1(p, sep) || result(std::list<A>());
}

template <class A, class B, class C>
Parser<B> between(Parser<A> const& left, Parser<B> const& p, Parser<C> const& right)
{
    return left  >= [p, right](A _){
    return p     >= [right](const B x){
    return right >= [x](const C _){
    return result(x);};};};
}

template <class A>
Parser<A> chainl(Parser<A> const& p, Parser< std::function<A(A, A)> > const& op)
{
    using f_t = std::function<A(A, A)>;
    const Parser <std::pair<f_t, A>> chunk = op   >= [p](const f_t f){
                                     return  p    >= [f](const A x){
                                     return result(std::make_pair(f, x));};};

    return p           >= [chunk](const A x){
    return many(chunk) >= [x](const std::list< std::pair<f_t, A> > fys){
        A acc = x;
        for (auto fy : fys)
        {
            acc = fy.first(acc, fy.second);
        }
    return result(acc);};};
}

//template <class A>
//Parser<A> chainl(Parser<A> const& p, Parser< std::function<A(A, A)> > const& op)
//{
//    using f_t = std::function<A(A, A)>;
//    std::function<Parser<A>(A)> rest = [p, op, &rest](const A x) -> Parser<A> // referencing on function stack is bad idea
//    {
//        return  (op  >= [p, x, &rest](const f_t f){
//        return   p   >= [f, x, &rest](const A y){
//        return   rest(f(x, y));};}) || result(x);
//    };
//    return p >= rest;
//}



#endif // COMBINATORS_H

