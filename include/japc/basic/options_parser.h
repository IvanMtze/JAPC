#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H
#include "japc/basic/file.h"
#include <getopt.h> /* for getopt_long; POSIX standard getopt is in unistd.h */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

namespace Pascal
{
struct WarningOptions
{
    unsigned int wall : 1;
    unsigned int werror : 1;
    unsigned int wunimplemented : 1;
    unsigned int invalid : 1;
};
struct ParsedOptions
{
    std::vector<std::string> files;
    std::vector<std::string> include;
    struct WarningOptions warningOptions;
    std::string output;
};
ParsedOptions parseJapcOptions(int argc, char **argv);
void parseWarningOptions(char *optarg, struct WarningOptions &warningOptions);
void print_help();
void parseIncludeOptions(char *optarg, struct ParsedOptions &parsedOptions);
} // namespace Pascal
#endif