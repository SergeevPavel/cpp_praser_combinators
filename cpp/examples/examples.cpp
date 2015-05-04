#include <iostream>
#include <vector>
#include <list>

#include "combinators.h"
#include "concrete_combinators.h"

#include "utils.h"

void result_test()
{
    std::cout << "result test:" << std::endl;

    std::cout << result<int>(5).apply("hello") << std::endl;
    std::cout << result<std::string>("x").apply("hello") << std::endl;
    std::cout << result<char>('t').apply("hello") << std::endl;

    std::cout << "----------------" << std::endl;
}

void zero_test()
{
    std::cout << "zero test:" << std::endl;

    std::cout << zero<int>().apply("hello") << std::endl;

    std::cout << "----------------" << std::endl;
}

void item_test()
{
    std::cout << "item test:" << std::endl;

    std::cout << item().apply("hello") << std::endl;

    std::cout << "----------------" << std::endl;
}

void bind_test()
{
    std::cout << "bind test:" << std::endl;

    Parser<std::string> p = item() >= [](char x1){
                     return item() >= [x1](char x2){
                     return result("<" + std::string(1, x1) + ">" + "<" + std::string(1, x2) + ">");
        };
    };

    std::cout << p.apply("hello") << std::endl;

    std::cout << "----------------" << std::endl;
}

void satisfy_test()
{
    std::cout << "satisfy test:" << std::endl;
    auto is_digit = [](const char c){return c >= '0' && c <= '9';};
    std::cout << satisfy(is_digit).apply("123") << std::endl;
    std::cout << satisfy(is_digit).apply("a1b") << std::endl;

    std::cout << symbol('z').apply("zzz") << std::endl;
    std::cout << symbol('a').apply("xyz") << std::endl;

    std::cout << letter().apply("asda") << std::endl;
    std::cout << letter().apply("AsdA") << std::endl;
    std::cout << letter().apply("123A") << std::endl;

    std::cout << "----------------" << std::endl;
}

void word_test()
{
    std::cout << "word test:" << std::endl;
    std::cout << word().apply("Yes!") << std::endl;
    std::cout << "----------------" << std::endl;
}

void string_test()
{
    std::cout << "string test:" << std::endl;
    std::cout << string("abc").apply("abcd") << std::endl;
    std::cout << string("abc").apply("xabc") << std::endl;
    std::cout << "----------------" << std::endl;
}

void many_test()
{
    std::cout << "many test:" << std::endl;
    std::cout << many(digit()).apply("abcd") << std::endl;
    std::cout << many(digit()).apply("1234dasd") << std::endl;
    std::cout << "----------------" << std::endl;
}

void many1_test()
{
    std::cout << "many1 test:" << std::endl;
    std::cout << many1(digit()).apply("abcd") << std::endl;
    std::cout << many1(digit()).apply("1234dasd") << std::endl;
    std::cout << "----------------" << std::endl;
}

void nat_test()
{
    std::cout << "nat test:" << std::endl;
    std::cout << nat().apply("123abc") << std::endl;
    std::cout << nat().apply("abc") << std::endl;
    std::cout << "----------------" << std::endl;
}

void integer_test()
{
    std::cout << "integer test:" << std::endl;
    std::cout << integer().apply("00123abc") << std::endl;
    std::cout << integer().apply("-123abc") << std::endl;
    std::cout << integer().apply("abc") << std::endl;
    std::cout << "----------------" << std::endl;
}

void sepby_test()
{
    std::cout << "sepby test:" << std::endl;
    std::cout << sepby(integer(), symbol(',')).apply("abcd") << std::endl;
    std::cout << sepby(integer(), symbol(',')).apply("12,42,64,38 abcd") << std::endl;
    std::cout << "----------------" << std::endl;
}

void between_test()
{
    std::cout << "between test:" << std::endl;
    Parser<std::list<int>> ints = between(symbol('['), sepby(integer(), symbol(',')), symbol(']'));
    std::cout << ints.apply("[-2,34,23,56]") << std::endl;
    std::cout << "----------------" << std::endl;
}

void chainl_test()
{
    std::cout << "chainl test:" << std::endl;
    const Parser<std::function<int(int,int)>> plus_op = symbol('+') >= [](const char _){
        return result(std::function<int(int,int)>([](const int a, const int b){ return a + b;}));
    };

    std::cout << chainl(integer(), plus_op).apply("22") << std::endl;
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
    many_test();
    many1_test();
    nat_test();
    integer_test();
    sepby_test();
    between_test();
    chainl_test();
    return 0;
}
