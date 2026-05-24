#include <iostream>
#include <string>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <Windows.h>
#include <psapi.h>
#include <vector>
#include <utility>
#include <memory>
#include <filesystem>

struct HandleCloser {
    void operator()(HANDLE h) const {
        if (h != NULL && h != INVALID_HANDLE_VALUE) {
            CloseHandle(h);
        }
    }
};

using ScopedHandle = std::unique_ptr<void, HandleCloser>;

std::int64_t get_process_memory(HANDLE pid)
{
    if (pid == NULL)
        throw std::runtime_error("Invalid pid");
    PROCESS_MEMORY_COUNTERS pmc;
    if (!GetProcessMemoryInfo(pid, &pmc, sizeof(pmc)))
        throw std::runtime_error("GetProcessMemoryInfo failed: " + std::to_string(GetLastError()));
    return pmc.WorkingSetSize;
}

HANDLE start_process(
    const std::string& filename,
    const std::string& input_file,
    const std::string& output_file
)
{
    std::cout << "Starting process: " << filename << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
        // check if input file exists
    if (!std::filesystem::exists(input_file)){
        throw std::runtime_error("Input file does not exist: " + input_file);
    }

    ScopedHandle hJob(CreateJobObject(NULL, NULL), HandleCloser{});
    if (!hJob)
        throw std::runtime_error("CreateJobObject failed: " + std::to_string(GetLastError()));

    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = {0};
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(hJob.get(), JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
        throw std::runtime_error("SetInformationJobObject failed: " + std::to_string(GetLastError()));

    SECURITY_ATTRIBUTES sa = {0};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    ScopedHandle hInputFile(CreateFile(input_file.c_str(), GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL), HandleCloser{});
    if (!hInputFile)
        throw std::runtime_error("CreateFile input failed: " + std::to_string(GetLastError()));

    ScopedHandle hOutputFile(CreateFile(output_file.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL), HandleCloser{});
    if (!hOutputFile)
        throw std::runtime_error("CreateFile output failed: " + std::to_string(GetLastError()));

    STARTUPINFO si = {0};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = hInputFile.get();
    si.hStdOutput = hOutputFile.get();
    si.hStdError = hOutputFile.get();

    PROCESS_INFORMATION pi = {0};

    std::string cmd = "\"" + filename + "\"";
    std::vector<char> cmdline(cmd.begin(), cmd.end());
    cmdline.push_back('\0');

    if (!CreateProcess(
            NULL,
            cmdline.data(),
            NULL,
            NULL,
            TRUE,
            CREATE_SUSPENDED | CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi))
    {
        throw std::runtime_error("CreateProcess failed: " + std::to_string(GetLastError()));
    }

    ScopedHandle hProcess(pi.hProcess, HandleCloser{});
    ScopedHandle hThread(pi.hThread, HandleCloser{});

    if (!AssignProcessToJobObject(hJob.get(), hProcess.get())) {
        TerminateProcess(hProcess.get(), 1);
        throw std::runtime_error("AssignProcessToJobObject failed: " + std::to_string(GetLastError()));
    }

    if (ResumeThread(hThread.get()) == (DWORD)-1) {
        TerminateProcess(hProcess.get(), 1);
        throw std::runtime_error("ResumeThread failed: " + std::to_string(GetLastError()));
    }

    // Transfer ownership of the process handle to the caller.
    // Note: hJob created with JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE means:
    // when this function returns, hJob terminates all child processes on close.
    // This is intentional: job cleanup is handled by OS.
    // The thread handle is not returned and is closed automatically when hThread
    // goes out of scope.
    HANDLE result = hProcess.release();
    (void)hJob.release();  // hJob closed by OS via JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE

    return result;
}

bool stop_process(HANDLE pid)
{
    // Caller responsibility: must call stop_process() to release HANDLE ownership.
    // Failing to call this function results in HANDLE leak.
    if (!pid) return false;
    bool result = TerminateProcess(pid, 0) != 0;
    CloseHandle(pid);
    return result;
}

bool is_up_process(HANDLE pid)
{
    if (!pid) return false;
    DWORD exit_code = 0;
    if (!GetExitCodeProcess(pid, &exit_code))
        throw std::runtime_error("GetExitCodeProcess failed: " + std::to_string(GetLastError()));
    return exit_code == STILL_ACTIVE;
}

bool apply_cgroup_limit(HANDLE /*pid*/, size_t /*limit*/)
{
    return false;
}

void remove_cgroup(HANDLE /*pid*/)
{
}
