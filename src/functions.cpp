#include <iostream>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

#include "definitions.h"

#if defined(IS_WINDOWS)
// window code
// include windows headers
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#else
#if defined(IS_LINUX)
// linux code
// include linux headers
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/resource.h>
#endif
#endif

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
    std::cout << "execute application:" << std::endl;
    std::cout << application << " input: " << input << " output: " << output << std::endl;
    // system(application.c_str());

    auto start = std::chrono::high_resolution_clock::now();

    std::string execution_line = application + " < " + input + " > " + output;

    std::size_t pid = start_application(application);

    while()
    std::size_t memoryUsed = get_memory_usage(pid);

    auto stop = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "Elapsed time: " << elapsed << " milliseconds, memory used = " << memoryUsed << std::endl;
}

pid_type start_application(std::string app_name)
{
    pid_type pid = 0;
#if defined(IS_WINDOWS)
    // window code
        STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL,   // No module name (use command line)
        (LPSTR)app_name.c_str(),   // Command line
        NULL,   // Process handle not inheritable
        NULL,   // Thread handle not inheritable
        FALSE,  // Set handle inheritance to FALSE
        0,      // No creation flags
        NULL,   // Use parent's environment block
        NULL,   // Use parent's starting directory 
        &si,    // Pointer to STARTUPINFO structure
        &pi)    // Pointer to PROCESS_INFORMATION structure
        )
    {
        // cout << "Failed to create process: " << GetLastError() << endl;
        return NULL;
    }

    pid = pi.hProcess;
#else
    pid = fork();

    if (pid == -1) {
        // cout << "Failed to create process." << endl;
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process
        execl(app_name.c_str(), app_name.c_str(), NULL);
        // cout << "Failed to execute process." << endl;
        exit(EXIT_FAILURE);
    }
    else {
        // Parent process
        return pid;
    }
#endif
    return pid;
}

int get_memory_usage(pid_type pid)
{
    int memory = 0;
#if defined(IS_WINDOWS)
    PROCESS_MEMORY_COUNTERS_EX pmc;

    if (!GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
        // cout << "Failed to get process memory information: " << GetLastError() << endl;
        return memory;
    }

    memory = pmc.PrivateUsage;
#else
    rusage usage;
    if (getrusage(RUSAGE_CHILDREN, &usage) == -1) {
        // cout << "Failed to get process memory information." << endl;
        return memory;
    }

    memory = usage.ru_maxrss;
#endif
    return memory;
}

int get_time_usage(pid_type pid)
{
    return 0;
}