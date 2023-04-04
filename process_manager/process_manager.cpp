#include "process_manager.h"

#include <Windows.h>
#include <psapi.h>
#include <iostream>
#include <thread>

std::uint64_t get_work_time(HANDLE h)
{
    uint64_t totalMs = 0;
    FILETIME createTime, exitTime, kernelTime, userTime;

    if (GetProcessTimes(h, &createTime, &exitTime, &kernelTime, &userTime))
    {
        ULARGE_INTEGER kernelTimeUL, userTimeUL;
        kernelTimeUL.LowPart = kernelTime.dwLowDateTime;
        kernelTimeUL.HighPart = kernelTime.dwHighDateTime;
        userTimeUL.LowPart = userTime.dwLowDateTime;
        userTimeUL.HighPart = userTime.dwHighDateTime;

        // totalMs - время работы процесса в миллисекундах
        totalMs = (kernelTimeUL.QuadPart + userTimeUL.QuadPart) / (10000);
    }
    return totalMs;
}

void process_manager::set_config(const process_manager::config &cfg)
{
    this->cfg = cfg;
}

process_manager::result_info process_manager::start_app(
    const std::string &managed_app,
    const std::string &input_file,
    const std::string &output_file)
{
    this->managed_app = managed_app;
    this->input_file = input_file;
    this->output_file = output_file;
    std::string command = this->managed_app +
                          " < " + this->input_file +
                          " > " + this->output_file;

    std::cout << "[debug] command: " << command << std::endl;

    process_manager::result_info result = {STATUS::OK, "ok"};

    auto process_id = this->create_process(command);

    while (this->is_process_up(process_id))
    {
        // check time limit
        if (this->is_time_limit(process_id))
        {
            // prepare error, break
            result.status_code = STATUS::TIME_LIMIT;
            result.status_message = "time limit exceed: ";
            // stop process
            this->close_process(process_id, ERROR_TIMEOUT);
            return result;
        }
        if (this->is_memory_limit(process_id))
        {
            // prepare error, break
            result.status_code = STATUS::MEMORY_LIMIT;
            result.status_message = "memory limit exceed: " +
                                    std::to_string(this->maxed.memory_limit / 1024) + "KB";
            // stop process
            this->close_process(process_id, ERROR_OUTOFMEMORY);
            return result;
        }
        // std::this_thread::sleep_for(milliseconds(1));
    }
    return result;
}

const process_manager::config& process_manager::used() const {
    return this->maxed;
}

int64_t process_manager::create_process(const std::string &command)
{
    // Windows solution
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(
            NULL,                   // No module name (use command line)
            (LPSTR)command.c_str(), // Command line
            NULL,                   // Process handle not inheritable
            NULL,                   // Thread handle not inheritable
            FALSE,                  // Set handle inheritance to FALSE
            0,                      // No creation flags
            NULL,                   // Use parent's environment block
            NULL,                   // Use parent's starting directory
            &si,                    // Pointer to STARTUPINFO structure
            &pi                     // Pointer to PROCESS_INFORMATION structure
            ))
    {
        // cout << "Failed to create process: " << GetLastError() << endl;
        return (int64_t)NULL;
    }

    return (int64_t)(pi.hProcess);
}

void process_manager::close_process(int64_t pid, uint16_t code) const
{
    // Windows solution
    HANDLE handle = (HANDLE)pid;
    TerminateProcess(handle, code);
}

bool process_manager::is_memory_limit(int64_t pid)
{
    // Windows solution
    HANDLE handle = (HANDLE)pid;
    PROCESS_MEMORY_COUNTERS_EX pmc;

    if (!GetProcessMemoryInfo(handle, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
    {
        std::cout << "[debug] Failed to get process memory information: " << GetLastError() << std::endl;
        return true;
    }

    if (pmc.PrivateUsage > this->maxed.memory_limit)
    {
        this->maxed.memory_limit = pmc.PrivateUsage;
    }
    // std::cout << "[debug] Private Usage: " << pmc.PrivateUsage << std::endl;
    // std::cout << "[debug] Working Set Size: " << pmc.WorkingSetSize << std::endl;
    if (pmc.PrivateUsage > cfg.memory_limit)
    {
        return true;
    }
    return false;
}

bool process_manager::is_time_limit(int64_t pid)
{
    uint64_t ms = get_work_time((HANDLE)pid);
    this->maxed.time_limit = ms;
    return this->maxed.time_limit > this->cfg.time_limit;
}

bool process_manager::is_process_up(int64_t pid) const
{
    // Windows solution
    DWORD exit_code;
    HANDLE handle = (HANDLE)pid;
    GetExitCodeProcess(handle, &exit_code);
    return exit_code == STILL_ACTIVE;
}
