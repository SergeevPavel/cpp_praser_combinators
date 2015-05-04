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


Parser<int> expr()
{
    return chainl(term(), plus_op || minus_op);
}

Parser<int> term()
{
    return chainl(factor(), mul_op);
}

Parser<int> factor()
{
    return integer() || between(symbol('('), expr(), symbol(')'));
}

int main()
{
    std::cout << expr().apply("2+2*3") << std::endl;
    return 0;
}
