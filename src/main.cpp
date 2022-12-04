#include <getopt.h>
#include <iostream>
#include <string>

#include "definitions.h"

int main(int argc, char **argv)
{
    int memory;
    double time;
    std::string application;
    std::string input = "";
    std::string output = "out.txt";

    int result;
    int option_index;

    while ((result = getopt_long(argc, argv, short_opts, long_opts, &option_index)) != -1)
    {
        switch (result)
        {
        case 'h':
            print_usage(argv[0]);
            return 0;
            break;
        case 'a':
            application = optarg;
            std::cout << application << std::endl;
            break;
        case 'm':
            memory = atoi(optarg);
            std::cout << memory << std::endl;
            break;
        case 't':
            time = atoi(optarg);
            std::cout << time << std::endl;
            break;
        case 'i':
            input = optarg;
            std::cout << input << std::endl;
            break;
        case 'o':
            output = optarg;
            std::cout << output << std::endl;
            break;
        default:
            break;
        }
    }

    return 0;
}
