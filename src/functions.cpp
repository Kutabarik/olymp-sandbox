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
#include "sys/types.h"
#include "sys/sysinfo.h"
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

    int result = system(execution_line.c_str());

    if (result != 0)
    {
        std::cerr << "Error running app" << std::endl;
    }

    std::size_t pid = get_pid(application);

    auto stop = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();

    std::cout << "Elapsed time: " << elapsed << " milliseconds" << std::endl;
}

pid_type get_pid(std::string app_name)
{
    pid_type pid = 0;
#if defined(IS_WINDOWS)
    // window code
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot)
    {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe32))
        {
            do
            {
                if (app_name == pe32.szExeFile)
                {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
#else
// linux code
#endif
    return pid;
}

int get_memory_usage(pid_type pid)
{
#if defined(IS_WINDOWS)
    HANDLE h = OpenProcess(PROCESS_QUERY_INFORMATION, false, pid);
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(h, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
#else
// linux code
#endif
    return 0;
}

int get_time_usage(pid_type pid)
{
    return 0;
}