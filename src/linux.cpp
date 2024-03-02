#include <cstddef>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <sstream>
#include <string>
#include <thread>
#include <sys/sysinfo.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @brief Get the consumed memory by process
 *
 * @param pid
 * @return std::int64_t
 */
std::int64_t get_process_memory(pid_t pid)
{
    std::string path = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(path);
    if (!statusFile)
    {
        std::cout << "[error] cant open file /proc/" << pid << "/status" << std::endl;
        return -1; // Error: Unable to retrieve memory usage
    }
    std::string line;
    while (std::getline(statusFile, line))
    {
        if (line.substr(0, 6) == "VmSize")
        {
            std::istringstream iss(line);
            std::string label, value;
            iss >> label >> value;
            return std::stol(value);
        }
    }
    return -2; // Error: memory usage not found
}

pid_t start_process(
    const std::string& filename,
    const std::string& input_file,
    const std::string& output_file)
{
    pid_t pid = fork();
    // parent process obtains pid of child process
    if (pid != 0)
    {
        return pid;
    }
    else
    {
        // child process obtains pid == 0
        std::cout << "[info] child process " << getpid() << std::endl;

        // int in = open(input_file.c_str(), O_RDONLY);
        // int out = open(output_file.c_str(), O_WRONLY);
        // dup2(in, STDIN_FILENO);
        // dup2(out, STDOUT_FILENO);
        execv(filename.c_str(), nullptr);
        std::cout << "[info] child process " << getpid() << std::endl;
        abort();
    }
}

bool is_up_process(pid_t pid)
{
    return kill(pid, 0) == 0;
}

bool stop_process(pid_t pid)
{
    return kill(pid, SIGTERM) == 0;
}