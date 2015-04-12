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
    const Parser<std::string> neWord = bind<char, std::string>(letter(), [](const char x){
                                return bind<std::string, std::string>(word(), [x](const std::string xs){
                                return result(x + xs);
        });
    });
    return plus<std::string>(neWord, result<std::string>(""));
}

Parser<std::string> string(const std::string pattern)
{
    if (pattern.length() == 0)
    {
        return result<std::string>("");
    }
    else
    {
        const char x = pattern[0];
        const std::string xs = pattern.substr(1, pattern.length() - 1);
        return bind<char, std::string>(symbol(x), [x, xs](const char _){
        return bind<std::string, std::string>(string(xs), [x, xs](const std::string _){
               return result(x + xs);
            });
        });
    }
}


#endif // CONCRETE_COMBINATORS

