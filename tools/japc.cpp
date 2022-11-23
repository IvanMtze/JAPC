//
// Created by wuser on 3/07/22.
//

#include "japc/basic/compiler.h"
#include "japc/basic/options_parser.h"

using namespace Pascal;
int main(int argc, char **argv)
{
    /**
     * @author wuser
     * The main program for japc
     */
    ParsedOptions parsedOptions = parseJapcOptions(argc, argv);
    Compiler compiler(parsedOptions);
    compiler.compile();
    return 0;
}