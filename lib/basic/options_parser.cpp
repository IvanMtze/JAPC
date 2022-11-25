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
            parseWarningOptions(optarg, parsedOptions.warningOptions);
            break;
        case 'I':
            parseIncludeOptions(optarg, parsedOptions);
            break;
        case 'h':
            print_help();
            break;
        case 'o':
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
    if(!strcmp(optarg, "all")){
        warningOptions.wall = 1;
    }else if(!strcmp(optarg, "unimplemented")){
        warningOptions.wunimplemented = 1;
    }else if(!strcmp(optarg, "error")){
        warningOptions.werror = 1;
    }else{
        warningOptions.invalid = 1;
        printf("Invalid option argument %s in -w\n", optarg);
        exit(1);
    }
};

void Pascal::print_help(){
    printf("This help is to be done!\n");
}
void Pascal::parseIncludeOptions(char* optarg,struct ParsedOptions &parsedOptions){
    parsedOptions.include.push_back(std::string(optarg));
}