#include <iostream>
#include <list>
#include <functional>

#include "combinators.h"
#include "concrete_combinators.h"

#include "utils.h"

const Parser<std::function<int(int,int)>> plus_op = symbol('+') >= [](const char _){
    return result(std::function<int(int,int)>([](const int a, const int b){ return a + b;}));
};

const Parser<std::function<int(int,int)>> minus_op = symbol('-') >= [](const char _){
    return result(std::function<int(int,int)>([](const int a, const int b){ return a - b; }));
};

const Parser<std::function<int(int,int)>> mul_op = symbol('*') >= [](const char _){
    return result(std::function<int(int,int)>([](const int a, const int b){ return a * b; }));
};

Parser<int> expr();
Parser<int> term();
Parser<int> factor();
Parser<int> lexpr = zero<int>();

Parser<int> expr()
{
    static bool check = false;
    static Parser<int> result = zero<int>();
    if (!check)
    {
        result = chainl(term(), plus_op || minus_op);
        check = true;
    }
    return result;
}

Parser<int> term()
{
    return chainl(factor(), mul_op);
}

Parser<int> factor()
{
    return integer() || between(symbol('('), lexpr, symbol(')'));
}


// 1) попробовать использовать пойнтеры и ссылки на парсеры
// 2) писать комбинаторы которые возвращают лямбды

int main()
{
    lexpr.copy_context(expr());
    while (true)
    {
        std::string str;
        std::cin >> str;
        std::cout << expr().apply(str) << std::endl;
    }
    return 0;
}
