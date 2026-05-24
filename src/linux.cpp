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

namespace {

class ScopedFd {
public:
    ScopedFd() : fd_(-1) {}
    explicit ScopedFd(int fd) : fd_(fd) {}

    ~ScopedFd() {
        if (fd_ >= 0) {
            close(fd_);
        }
    }

    ScopedFd(const ScopedFd&) = delete;
    ScopedFd& operator=(const ScopedFd&) = delete;

    ScopedFd(ScopedFd&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
    }

    ScopedFd& operator=(ScopedFd&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) {
                close(fd_);
            }
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }

    int get() const {
        return fd_;
    }

    int release() {
        int tmp = fd_;
        fd_ = -1;
        return tmp;
    }

    explicit operator bool() const {
        return fd_ >= 0;
    }

private:
    int fd_;
};

}  // namespace

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

        int in_fd = open(input_file.c_str(), O_RDONLY);
        int out_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (in_fd < 0 || out_fd < 0) {
            std::cerr << "[error] open file failed" << std::endl;
            abort();
        }
        dup2(in_fd, STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        dup2(out_fd, STDERR_FILENO);
        close(in_fd);
        close(out_fd);
        char *const argv[] = {const_cast<char*>(filename.c_str()), nullptr};
        execv(filename.c_str(), argv);
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