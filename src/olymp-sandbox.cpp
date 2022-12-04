
#include <getopt.h>
#include <iostream>
#include <string>
#include <thread>

<<<<<<< HEAD:src/olymp-sandbox.cpp
void print_usage(const std::string appname);
void start_app (std::string application, int memory, double time);

const char *short_opts = "a:i:o:h";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"memory", required_argument, 0, 'm'},
    {"time", required_argument, 0, 't'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};
=======
#include "definitions.h"
>>>>>>> 765bc5b213825c7fd178aaba8cf7bd034820eff4:src/main.cpp

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

    std::thread th(start_app, application, memory, time);

    std::cout << "Main programm" << std::endl;

    th.join();
    return 0;
}
<<<<<<< HEAD:src/olymp-sandbox.cpp

void start_app (std::string application, int memory, double time)
{
    system(application.c_str());
}


void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
}
=======
>>>>>>> 765bc5b213825c7fd178aaba8cf7bd034820eff4:src/main.cpp
