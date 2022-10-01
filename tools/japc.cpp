//
// Created by wuser on 3/07/22.
//

#include "scanner.h"
#include "parser.h"
#include <iostream>
int main()
{
    using namespace Pascal;
    Scanner scanner;
    scanner.setSkipTrivia(false);
    scanner.setTextSource("Program{*\"JAPC\"*};");
    TokenType type;
    Parser parser = Parser(std::make_unique<Scanner>(scanner));
    parser.parseFile();
    return 0;
}