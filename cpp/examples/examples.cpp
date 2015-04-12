#include <iostream>

#include "combinators.h"
#include "concrete_combinators.h"

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

void plus_test()
{
    std::cout << "plus test:" << std::endl;



//    auto out = p.apply("hello");

//    print<std::string>(out);

    std::cout << "----------------" << std::endl;
}

void satisfy_test()
{
    std::cout << "satisfy test:" << std::endl;
    auto is_digit = [](const char c){return c >= '0' && c <= '9';};
    print<char>(satisfy(is_digit).apply("123"));
    print<char>(satisfy(is_digit).apply("a1b"));

    print<char>(symbol('z').apply("zzz"));
    print<char>(symbol('a').apply("xyz"));

    print<char>(letter().apply("asda"));
    print<char>(letter().apply("AsdA"));
    print<char>(letter().apply("123A"));

    std::cout << "----------------" << std::endl;
}

void word_test()
{
    std::cout << "word test:" << std::endl;
    print<std::string>(word().apply("Yes!"));
    std::cout << "----------------" << std::endl;
}

void string_test()
{
    std::cout << "string test:" << std::endl;
    print<std::string>(string("abc").apply("abcd"));
    print<std::string>(string("abc").apply("xabc"));
    std::cout << "----------------" << std::endl;
}

int main()
{
    result_test();
    zero_test();
    item_test();
    bind_test();
    satisfy_test();
    word_test();
    string_test();
    return 0;
}
