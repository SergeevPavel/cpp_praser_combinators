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

void result_test()
{
    std::cout << "result test:" << std::endl;

    print<int>(result<int>(5).apply("hello"));
    print<std::string>(result<std::string>("x").apply("hello"));

    std::cout << "----------------" << std::endl;
}

void zero_test()
{
    std::cout << "zero test:" << std::endl;

    print<int>(zero<int>().apply("hello"));

    std::cout << "----------------" << std::endl;
}

void item_test()
{
    std::cout << "item test:" << std::endl;

    print<char>(item().apply("hello"));

    std::cout << "----------------" << std::endl;
}

int main()
{
    result_test();
    zero_test();
    item_test();
    return 0;
}
