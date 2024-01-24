#include <string>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <Windows.h>
#include <psapi.h>

std::int64_t get_process_memory(HANDLE pid)
{
    if (!pid)
    {
        return -1;
    }

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(pid, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

    return pmc.PrivateUsage;
}

HANDLE start_process(
    const std::string& filename,
    const std::string& input_file,
    const std::string& output_file)
{
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    memset(&piProcInfo, 0, sizeof(PROCESS_INFORMATION));
    memset(&siStartInfo, 0, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    // Redirect input and output to files
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    // std::wstring input(input_file.begin(), input_file.end());
    siStartInfo.hStdInput = CreateFile(input_file.c_str(),
                                       GENERIC_READ, 0, NULL, OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL, NULL);

    siStartInfo.hStdOutput = CreateFile(output_file.c_str(),
                                        GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                                        FILE_ATTRIBUTE_NORMAL, NULL);

    // Create the process
    if (!CreateProcess(NULL, const_cast<LPSTR>(filename.c_str()),
                       NULL, NULL, TRUE, 0,
                       NULL, NULL, &siStartInfo, &piProcInfo))
    {
        // Failed to create process
        throw std::runtime_error("Failed to create process");
    }
    // Close the handles that the parent process does not need
    CloseHandle(piProcInfo.hThread);
    return piProcInfo.hProcess;
}

bool stop_process(HANDLE pid)
{
    bool result = TerminateProcess(pid, 0);
    CloseHandle(pid);
    return result;
}

bool is_up_process(HANDLE pid)
{
    DWORD exit_code;
    HANDLE handle = pid;
    GetExitCodeProcess(handle, &exit_code);
    return exit_code == STILL_ACTIVE;
}