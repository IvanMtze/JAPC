#include "japc/basic/options_parser.h"
using namespace Pascal;
ParsedOptions Pascal::parseJapcOptions(int argc, char **argv)
{
    int opt;
    struct ParsedOptions parsedOptions;
    while ((opt = getopt(argc, argv, "W:w:I:o:h")) != -1){
        switch (opt)
        {
        case 'W':
        case 'w':
            printf("Option w has arg: %s\n", optarg);
            parseWarningOptions(optarg, parsedOptions.warningOptions);
            break;
        case 'I':
            printf("Option I has arg: %s\n", optarg);
            parseIncludeOptions(optarg, parsedOptions);
            break;
        case 'h':
            print_help();
            break;
        case 'o':
            printf("Option o was provided\n");
            parsedOptions.output = std::string(optarg);
            break;
        }
    }
    if (optind < argc)
    {
        while (optind < argc)
        {
            std::string nonarg = std::string(argv[optind++]);
            parsedOptions.files.push_back(nonarg);
        }
    }
    return parsedOptions;
}

void Pascal::parseWarningOptions(char* optarg, struct WarningOptions &warningOptions){
    if(strcmp(optarg, "all")){
        warningOptions.wall = 1;
    }else if(strcmp(optarg, "unimplemented")){
        warningOptions.wunimplemented = 1;
    }else if(strcmp(optarg, "error")){
        warningOptions.werror = 1;
    }else{
        warningOptions.invalid = 1;
        printf("Invalid option argument in -w\n");
    }
};

void Pascal::print_help(){

}
void Pascal::parseIncludeOptions(char* optarg,struct ParsedOptions &parsedOptions){
    parsedOptions.include.push_back(std::string(optarg));
}