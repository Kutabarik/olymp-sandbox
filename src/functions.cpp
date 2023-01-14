#include <iostream>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

#include "definitions.h"

void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
}

void print_status(STATUS_TYPE status)
{
    const std::string message[] = {
        "success",
        "fail",
        "time limit",
        "memory limit",
        "execution failed",
    };
    std::cout << message[(int)status] << std::endl;
}

void run_task(std::string application, std::string input, std::string output)
{
    std::cout << "execute application:" << application << std::endl;
    std::cout << " input: " << input << " output: " << output << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    int result = system(application.c_str());

    if (result != 0)
    {
        std::cerr << "Error running app" << std::endl;
    }

    auto stop = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "Elapsed time: " << elapsed << " milliseconds" << std::endl;
}