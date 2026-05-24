#include <cstddef>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <sstream>
#include <string>
#include <thread>
#include <cstdio>
#include <cerrno>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>

namespace {

/**
 * @brief Helper to manage cgroup v2 memory limits.
 * Requires root privileges to create and modify cgroups.
 */
class CgroupManager {
public:
    static bool apply_memory_limit(pid_t pid, size_t limit_bytes) {
        const std::string base_path = "/sys/fs/cgroup";
        if (!std::filesystem::exists(base_path)) return false;

        try {
            std::string group_name = "sandbox_" + std::to_string(pid);
            std::filesystem::path group_path = std::filesystem::path(base_path) / group_name;

            // Create cgroup directory
            if (!std::filesystem::exists(group_path)) {
                if (!std::filesystem::create_directory(group_path)) return false;
            }

            // Set memory limit
            std::ofstream limit_file(group_path / "memory.max");
            if (!limit_file) return false;
            limit_file << limit_bytes;
            limit_file.close();

            // Add process to cgroup
            std::ofstream procs_file(group_path / "cgroup.procs");
            if (!procs_file) return false;
            procs_file << pid;
            procs_file.close();

            return true;
        } catch (...) {
            return false;
        }
    }

    static void remove_group(pid_t pid) {
        std::string group_name = "sandbox_" + std::to_string(pid);
        std::error_code ec;
        std::filesystem::remove_all("/sys/fs/cgroup/" + group_name, ec);
    }
};

}  // namespace
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
            return std::stol(value) * 1024;  // Convert kB to bytes
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
    if (pid < 0)
    {
        return 0;
    }
    // parent process obtains pid of child process
    if (pid != 0)
    {
        return pid;
    }
    else
    {
        // child process obtains pid == 0
        ScopedFd in_fd(open(input_file.c_str(), O_RDONLY));
        if (!in_fd)
        {
            perror("open input");
            _exit(127);
        }

        ScopedFd out_fd(open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644));
        if (!out_fd)
        {
            perror("open output");
            _exit(127);
        }

        if (dup2(in_fd.get(), STDIN_FILENO) == -1 ||
            dup2(out_fd.get(), STDOUT_FILENO) == -1 ||
            dup2(out_fd.get(), STDERR_FILENO) == -1)
        {
            perror("dup2");
            _exit(127);
        }

        char *const argv[] = {const_cast<char*>(filename.c_str()), nullptr};
        execvp(filename.c_str(), argv);
        perror("execvp");
        _exit(127);
    }
}

bool is_up_process(pid_t pid)
{
    int status;
    pid_t result = waitpid(pid, &status, WNOHANG);
    return result == 0;
}

bool stop_process(pid_t pid)
{
    if (kill(pid, 0) == 0 && kill(pid, SIGTERM) != 0)
    {
        return false;
    }

    int status = 0;
    pid_t waited = waitpid(pid, &status, 0);
    if (waited == -1)
    {
        return errno == ECHILD;
    }
    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status) == 0;
    }
    return false;
}

bool apply_cgroup_limit(pid_t pid, size_t limit)
{
    return CgroupManager::apply_memory_limit(pid, limit);
}

void remove_cgroup(pid_t pid)
{
    CgroupManager::remove_group(pid);
}