#include <iostream>

#include "combinators.h"

template<class A>
void print(typename Parser<A>::output_t const& result)
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
    print<char>(result<char>('t').apply("hello"));

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

void bind_test()
{
    std::cout << "bind test:" << std::endl;

    Parser<std::string> p = bind<char, std::string>(item(), [](char x1){
                     return bind<char, std::string>(item(), [x1](char x2){
                     return result<std::string>("<" + std::string(&x1) + ">"  + "<" + std::string(&x2) + ">");
        });
    });

    auto out = p.apply("hello");

    print<std::string>(out);

    std::cout << "----------------" << std::endl;
}

int main()
{
    result_test();
    zero_test();
    item_test();
    bind_test();
    return 0;
}
