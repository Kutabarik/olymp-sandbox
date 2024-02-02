#include <string>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <Windows.h>
#include <psapi.h>

/**
 * @brief Get the process memory object by process id.
 * 
 * @param pid 
 * @return std::int64_t 
 */
std::int64_t get_process_memory(HANDLE pid)
{
    // valid pid
    if (pid == NULL)
    {
        throw std::runtime_error("Invalid pid");
    }
    // get process memory
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(pid, &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

/**
 * @brief Start a process, specified by name / path. Redirect input and output to files.
 * 
 * @param filename 
 * @param input_file 
 * @param output_file 
 * @return HANDLE 
 */
HANDLE start_process(
    const std::string& filename,
    const std::string& input_file,
    const std::string& output_file)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    HANDLE hInputFile = CreateFile(
        input_file.c_str(),
        GENERIC_READ,
        0,
        &sa,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    HANDLE hOutputFile = CreateFile(
        output_file.c_str(),
        GENERIC_WRITE,
        0,
        &sa,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hOutputFile;
    si.hStdOutput = hOutputFile;
    si.hStdInput = hInputFile;
    si.dwFlags |= STARTF_USESTDHANDLES;

    CreateProcess(
        NULL,
        const_cast<char*>(filename.c_str()),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si,
        &pi
    );

    CloseHandle(hInputFile);
    CloseHandle(hOutputFile);

    return pi.hProcess;
}

/**
 * @brief Close a process.
 * 
 * @param pid 
 * @return true 
 * @return false 
 */
bool stop_process(HANDLE pid)
{
    bool result = TerminateProcess(pid, 0);
    CloseHandle(pid);
    return result;
}

/**
 * @brief Check if a process is still running.
 * 
 * @param pid 
 * @return true 
 * @return false 
 */
bool is_up_process(HANDLE pid)
{
    DWORD exit_code;
    GetExitCodeProcess(pid, &exit_code);
    return exit_code == STILL_ACTIVE;
}