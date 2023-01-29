#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "definitions.h"

void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
}

void print_status(STATUS_TYPE status, double time, int memory)
{
    const std::string message[] = {
        "success",
        "fail",
        "time limit",
        "memory limit",
        "execution failed",
    };

    std::cout << "{\"status\":\"" << message[(int)status] << "\",\"time\":" << time << ",\"memory\":" << memory << "}" << std::endl;
}

void run_task(std::string application, std::string input, std::string output, int memory, double time)
{
    std::cout << "execute application:" << application << std::endl;
    std::cout << " input: " << input << " output: " << output << std::endl;

    rlimit mem_limit;
    mem_limit.rlim_cur = memory * 1024 * 1024; // convert to bytes
    mem_limit.rlim_max = memory * 1024 * 1024;
    setrlimit(RLIMIT_AS, &mem_limit); // setting the maximum size of the virtual memory of the process (address space) in bytes.

    rlimit time_limit;
    time_limit.rlim_cur = time; // convert to seconds
    time_limit.rlim_max = time * 2;
    setrlimit(RLIMIT_CPU, &time_limit); // setting the CPU time limit in seconds

    // start the application
    int res = system(application.c_str());

    // check return value
    if (WIFEXITED(res)) // not equal to zero if the child process has completed successfully
    {
        int exit_status = WEXITSTATUS(res);
        if (exit_status == 0)
        {
            std::cout << "Task completed successfully!" << std::endl;
        }
        else
        {
            std::cout << "Task failed with exit code " << exit_status << std::endl;
        }
    }
    else if (WIFSIGNALED(res))
    {
        int term_sig = WTERMSIG(res);
        std::cout << "Task terminated by signal " << term_sig << std::endl;
    }

    fclose(stdin);
    fclose(stdout);
}