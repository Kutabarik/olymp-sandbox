#define _GNU_SOURCE

#include <cstddef>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <sstream>
#include <string>
#include <thread>
#include <cstdio>
#include <cerrno>
#include <sched.h>
#include <sys/mman.h>
#include <sys/mount.h>
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
    static std::filesystem::path group_path_for(pid_t pid) {
        return std::filesystem::path("/sys/fs/cgroup") / ("sandbox_" + std::to_string(pid));
    }

    static bool apply_memory_limit(pid_t pid, size_t limit_bytes) {
        const std::string base_path = "/sys/fs/cgroup";
        std::error_code ec;
        if (!std::filesystem::exists(base_path, ec) || ec) return false;

        try {
            std::filesystem::path group_path = group_path_for(pid);

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

    static bool did_oom_kill(pid_t pid) {
        std::ifstream events_file(group_path_for(pid) / "memory.events");
        if (!events_file) {
            return false;
        }

        std::string key;
        unsigned long long value = 0;
        while (events_file >> key >> value) {
            if (key == "oom_kill" || key == "oom_group_kill") {
                return value > 0;
            }
        }

        return false;
    }

    static void remove_group(pid_t pid) {
        std::error_code ec;
        std::filesystem::remove_all(group_path_for(pid), ec);
    }
};

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

struct clone_child_args {
    int in_fd;
    int out_fd;
    std::string filename;
};

static int clone_child(void *arg) {
    clone_child_args *a = static_cast<clone_child_args*>(arg);

    // Make mount namespace private so mounts don't propagate to parent
    if (mount("", "/", "", MS_PRIVATE | MS_REC, NULL) != 0) {
        _exit(124);
    }

    // Remount /proc to reflect the new PID namespace
    umount2("/proc", MNT_DETACH);
    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        _exit(124);
    }

    // Redirect stdin/stdout/stderr
    if (dup2(a->in_fd, STDIN_FILENO) < 0 ||
        dup2(a->out_fd, STDOUT_FILENO) < 0 ||
        dup2(a->out_fd, STDERR_FILENO) < 0) {
        _exit(125);
    }
    close(a->in_fd);
    close(a->out_fd);

    // Drop privileges to nobody
    if (setgid(65534) != 0 || setuid(65534) != 0) {
        _exit(126);
    }

    // Execute target program
    char *const argv[] = {const_cast<char*>(a->filename.c_str()), nullptr};
    execv(a->filename.c_str(), argv);
    _exit(127);
}

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
    ScopedFd in_fd(open(input_file.c_str(), O_RDONLY));
    if (!in_fd)
    {
        return 0;
    }

    ScopedFd out_fd(open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644));
    if (!out_fd)
    {
        return 0;
    }

    // Try to create an isolated process using clone() with namespaces.
    // Requires root / CAP_SYS_ADMIN. Falls back to fork() if unavailable.
    {
        const size_t stack_size = 1024 * 1024;
        void *stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
        if (stack != MAP_FAILED)
        {
            clone_child_args args = {in_fd.get(), out_fd.get(), filename};
            pid_t pid = clone(clone_child, static_cast<char*>(stack) + stack_size,
                              CLONE_NEWPID | CLONE_NEWNS | CLONE_NEWNET | SIGCHLD,
                              &args);
            if (pid > 0)
            {
                // Namespaced process created successfully.
                // The child stack is intentionally not freed here; the kernel
                // reclaims it when the parent process exits.
                return pid;
            }
            // clone() failed (no privileges), clean up stack
            munmap(stack, stack_size);
        }
    }

    // Fallback to fork() when namespaces are unavailable
    pid_t pid = fork();
    if (pid < 0)
    {
        return 0;
    }
    if (pid != 0)
    {
        return pid;
    }

    // Child (fork path)
    ScopedFd child_in(open(input_file.c_str(), O_RDONLY));
    if (!child_in)
    {
        perror("open input");
        _exit(127);
    }

    ScopedFd child_out(open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644));
    if (!child_out)
    {
        perror("open output");
        _exit(127);
    }

    if (dup2(child_in.get(), STDIN_FILENO) == -1 ||
        dup2(child_out.get(), STDOUT_FILENO) == -1 ||
        dup2(child_out.get(), STDERR_FILENO) == -1)
    {
        perror("dup2");
        _exit(127);
    }

    char *const argv[] = {const_cast<char*>(filename.c_str()), nullptr};
    execv(filename.c_str(), argv);
    perror("execv");
    _exit(127);
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

bool did_cgroup_oom(pid_t pid)
{
    return CgroupManager::did_oom_kill(pid);
}

void remove_cgroup(pid_t pid)
{
    CgroupManager::remove_group(pid);
}
