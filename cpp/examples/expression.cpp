#include <iostream>
#include <list>
#include <functional>

#include "combinators.h"
#include "concrete_combinators.h"

#include "utils.h"

Parser<int> expr_p;

const Parser<std::function<int(int,int)>> plus_op = symbol('+') > result(std::function<int(int,int)>([](const int a, const int b){ return a + b;}));

const Parser<std::function<int(int,int)>> minus_op = symbol('-') > result(std::function<int(int,int)>([](const int a, const int b){ return a - b; }));

const Parser<std::function<int(int,int)>> mul_op = symbol('*') > result(std::function<int(int,int)>([](const int a, const int b){ return a * b; }));

Parser<int> factor()
{
    return integer() || between(symbol('('), indentation() > expr_p, indentation() > symbol(')'));
}

Parser<int> term()
{
    return chainl(indentation() > factor(), indentation() > mul_op);
}

Parser<int> expr()
{
    return chainl(indentation() > term(), indentation() > (plus_op || minus_op));
}

int main()
{
    expr_p.set_context(expr());
    while (true)
    {
        std::string str;
        std::getline( std::cin, str);
        std::cout << expr_p.apply(str) << std::endl;
    }
    return 0;
}
