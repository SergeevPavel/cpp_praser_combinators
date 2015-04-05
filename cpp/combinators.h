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
        return std::vector< std::pair<A, std::string> >(1, std::make_pair(v, input));
    });
}


#endif // COMBINATORS_H

