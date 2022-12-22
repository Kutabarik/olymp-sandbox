#include <getopt.h>
#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

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

    if (argc == 1)
        print_usage(argv[0]);

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
            break;
        case 'm':
            memory = atoi(optarg);
            break;
        case 't':
            time = atoi(optarg);
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        default:
            break;
        }
    }

    std::thread th(run_task, application, input, output);

    th.join();

    return 0;
}
