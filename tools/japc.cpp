//
// Created by wuser on 3/07/22.
//

#include "japc/basic/diagnostics.h"
#include "japc/parser/parser.h"
#include "japc/scanner/scanner.h"
int main()
{
    using namespace Pascal;
    Scanner scanner;
    scanner.setSkipTrivia(true);
    scanner.setTextSource(
        "Program amazing; import system only (write, read); wrapper; Function fuc(protected a,:int):int");
    TokenType type;
    JAPCDiagnostics diagnosticsEngine;
    Parser parser = Parser(std::make_unique<Scanner>(scanner), std::make_unique<JAPCDiagnostics>(diagnosticsEngine));
    parser.parseFile();
    return 0;
}