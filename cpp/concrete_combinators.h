#ifndef CONCRETE_COMBINATORS
#define CONCRETE_COMBINATORS

#include "parser.h"
#include "combinators.h"

Parser<char> symbol(char x)
{
    return satisfy([x](char y){ return x == y; });
}

Parser<char> digit()
{
    return satisfy([](char y){ return y >= '0' && y <= '9'; });
}

Parser<char> lower()
{
    return satisfy([](char y){ return y >= 'a' && y <= 'z'; });
}

Parser<char> upper()
{
    return satisfy([](char y){ return y >= 'A' && y <= 'Z'; });
}

Parser<char> letter()
{
    return plus<char>(lower(), upper());
}

Parser<std::string> word()
{
    const Parser<std::string> neWord = bind<char, std::string>(letter(), [](char x){
                                return bind<std::string, std::string>(word(), [x](std::string xs){
                                return result(x + xs);
        });
    });
    return plus<std::string>(neWord, result<std::string>(""));
}

#endif // CONCRETE_COMBINATORS

