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
    scanner.setSkipTrivia(true);
    scanner.setTextSource("Program amazing; import system only (write, read); wrapper; Function fuc(protected a,b:int):int");
    TokenType type;
    Parser parser = Parser(std::make_unique<Scanner>(scanner));
    parser.parseFile();
    return 0;
}