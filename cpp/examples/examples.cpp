#include <iostream>

#include "combinators.h"

template<class A>
void print(typename Parser<A>::parser_output_t const& result)
{
    std::cout << "[";
    for (auto p : result)
    {
        std::cout << "(" << p.first << "," << p.second << ")";
    }
    std::cout << "]" << std::endl;
}

int main()
{
    print<int>(result<int>(5).apply("hello"));
    return 0;
}
