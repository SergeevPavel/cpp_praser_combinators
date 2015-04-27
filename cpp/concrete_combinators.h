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
    return plus(lower(), upper());
}

Parser<char> alphanum()
{
    return plus(letter(), digit());
}

Parser<std::string> word()
{
    const Parser<std::string> neWord = bind(letter(), [](const char x){
                                return bind(word(), [x](const std::string xs){
                                return result(x + xs);
        });
    });
    return plus(neWord, result<std::string>(""));
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
        return bind(symbol(x), [x, xs](const char _){
        return bind(string(xs), [x, xs](const std::string _){
               return result(x + xs);
            });
        });
    }
}

Parser<int> nat()
{
    return bind(many1(digit()), [](const std::vector<char> xs){
        int acc = 0;
        for (char c : xs)
        {
            acc = acc * 10 + c - '0';
        }
        return result(acc);
    });
}

Parser<int> integer()
{
    const Parser<int> neg = bind(symbol('-'), [](const char _){
                     return bind(nat(), [](const int x){
                     return result(-x);
        });
    });
    return plus(neg, nat());
}

#endif // CONCRETE_COMBINATORS

