//
// Created by wuser on 3/07/22.
//

#include "scanner.h"
#include <iostream>
int main()
{
    using namespace Pascal;
    Scanner scanner = Scanner();
    scanner.setSkipTrivia(false);
    scanner.setTextSource("Program{*\"JAPC\"*};");
    TokenType type;
    do
    {
        type = scanner.scan();
        std::cout << scanner.tokenTypeToStr(type) << std::endl;
    } while (type != TokenType::END_OF_FILE);
    return 0;
}