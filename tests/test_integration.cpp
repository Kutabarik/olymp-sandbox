#include <catch2/catch_test_macros.hpp>

#include "process_manager.hpp"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <system_error>
#include <thread>
#include <cstring>
#if !defined(WIN32)
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace {

void remove_if_exists(const std::string& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
}

bool wait_for_non_empty_file(const std::string& path) {
    for (int i = 0; i < 30; ++i) {
        std::error_code ec;
        if (std::filesystem::exists(path, ec) && std::filesystem::file_size(path, ec) > 0) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return false;
}

class scoped_cleanup {
public:
    explicit scoped_cleanup(std::initializer_list<std::string> paths)
        : paths_(paths) {
        for (const auto& path : paths_) {
            remove_if_exists(path);
        }
    }

    ~scoped_cleanup() {
        for (const auto& path : paths_) {
            remove_if_exists(path);
        }
    }

private:
    std::vector<std::string> paths_;
};

std::string read_file(const std::string& path) {
    std::ifstream in(path);
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
}

std::string get_testapp_path() {
#ifdef TESTAPP_PATH
    const std::string app = TESTAPP_PATH;
#else
    const std::string app = "testapp";
#endif
    REQUIRE(std::filesystem::exists(app));
    return app;
}

mc::config make_cfg(const std::string& input,
                    const std::string& output,
                    std::size_t memory_limit,
                    std::size_t time_limit) {
    mc::config cfg;
    cfg.application = get_testapp_path();
    cfg.input = input;
    cfg.output = output;
    cfg.memory_limit = memory_limit;
    cfg.time_limit = time_limit;
    return cfg;
}

bool can_run_cgroup_test() {
#if defined(WIN32)
    return false;
#else
    if (geteuid() != 0) {
        return false;
    }

    std::error_code ec;
    const std::filesystem::path cgroup_root = "/sys/fs/cgroup";
    const std::filesystem::path controllers = cgroup_root / "cgroup.controllers";

    if (!std::filesystem::exists(cgroup_root, ec) ||
        !std::filesystem::exists(controllers, ec)) {
        return false;
    }

    std::ifstream in(controllers);
    if (!in) {
        return false;
    }

    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());

    return contents.find("memory") != std::string::npos;
#endif
}

}  // namespace

TEST_CASE("Integration: testapp is compiled", "[integration][9.1]") {
#if defined(WIN32)
    SKIP("Integration tests run on Linux.");
#endif

    const std::string app_path = get_testapp_path();
    REQUIRE(std::filesystem::is_regular_file(app_path));
    REQUIRE(std::filesystem::file_size(app_path) > 0);
}

TEST_CASE("Integration: testapp runs with valid input/output", "[integration][9.2]") {
#if defined(WIN32)
    SKIP("Integration tests run on Linux.");
#endif

    const std::string input_path = "integration_input_92.txt";
    const std::string output_path = "integration_output_92.txt";
    const std::string log_path = "integration_log_92.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "50 64";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 128ull * 1024ull * 1024ull, 1000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::OK);
    }

    REQUIRE(std::filesystem::exists(output_path));
    REQUIRE(wait_for_non_empty_file(output_path));

}

TEST_CASE("Integration: time limit is enforced", "[integration][9.3]") {
#if defined(WIN32)
    SKIP("Integration tests run on Linux.");
#endif

    const std::string input_path = "integration_input_93.txt";
    const std::string output_path = "integration_output_93.txt";
    const std::string log_path = "integration_log_93.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "250 64";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 512ull * 1024ull * 1024ull, 30),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::TIME_LIMIT);
    }

}

TEST_CASE("Integration: memory limit is enforced", "[integration][9.4]") {
#if defined(WIN32)
    SKIP("Integration tests run on Linux.");
#endif

    const std::string input_path = "integration_input_94.txt";
    const std::string output_path = "integration_output_94.txt";
    const std::string log_path = "integration_log_94.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "200 131072";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 1ull * 1024ull * 1024ull, 2000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::MEMORY_LIMIT);
    }

}

TEST_CASE("Integration: cgroup memory limit is enforced (privileged)", "[integration][13.4][privileged]") {
#if defined(WIN32)
    SKIP("Integration tests run on Linux.");
#endif

    if (!can_run_cgroup_test()) {
        SKIP("cgroup v2 memory controller is not available or not usable in this environment.");
    }

    const std::string input_path = "integration_input_13_4.txt";
    const std::string output_path = "integration_output_13_4.txt";
    const std::string log_path = "integration_log_13_4.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "200 131072"; // 128 MB
    }

    {
        // Set a very tight limit to force the cgroup memory controller to kill the process.
        mc::process_manager manager(
            make_cfg(input_path, output_path, 16ull * 1024ull * 1024ull, 2000),
            log_path);
        const mc::result_info result = manager.start_app();

        REQUIRE(result.status_code == mc::result_info::STATUS::MEMORY_LIMIT);
    }
}

TEST_CASE("Integration: stdin redirection works on Linux", "[integration][10.2]") {
#if defined(WIN32)
    SKIP("Linux-specific test");
#endif

    const std::string input_path = "integration_input_10.2.txt";
    const std::string output_path = "integration_output_10.2.txt";
    const std::string log_path = "integration_log_10.2.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "10 1";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 128ull * 1024ull * 1024ull, 1000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::OK);
    }

    REQUIRE(std::filesystem::exists(output_path));
    REQUIRE(wait_for_non_empty_file(output_path));
}

TEST_CASE("Integration: stdout redirection works on Linux", "[integration][10.3]") {
#if defined(WIN32)
    SKIP("Linux-specific test");
#endif

    const std::string input_path = "integration_input_10.3.txt";
    const std::string output_path = "integration_output_10.3.txt";
    const std::string log_path = "integration_log_10.3.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "10 1";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 128ull * 1024ull * 1024ull, 1000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::OK);
    }

    REQUIRE(std::filesystem::exists(output_path));
    const std::string content = read_file(output_path);
    REQUIRE(content.find("allocated 1 KB successfully.") != std::string::npos);
}

TEST_CASE("Integration: output file is created on Linux", "[integration][11.2]") {
#if defined(WIN32)
    SKIP("Linux-specific test");
#endif

    const std::string input_path = "integration_input_11.2.txt";
    const std::string output_path = "integration_output_11.2.txt";
    const std::string log_path = "integration_log_11.2.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "10 1";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 128ull * 1024ull * 1024ull, 1000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::OK);
    }

    REQUIRE(std::filesystem::exists(output_path));
}

#if !defined(WIN32)
// Forward declarations of platform functions from linux.cpp
pid_t start_process(const std::string& filename,
                    const std::string& input_file,
                    const std::string& output_file);
bool namespace_differs(const std::string& ns_type, pid_t child_pid) {
    std::string child_link = "/proc/" + std::to_string(child_pid) + "/ns/" + ns_type;
    std::string self_link = "/proc/self/ns/" + ns_type;
    char child_buf[256] = {}, self_buf[256] = {};
    if (readlink(child_link.c_str(), child_buf, sizeof(child_buf) - 1) < 0) return false;
    if (readlink(self_link.c_str(), self_buf, sizeof(self_buf) - 1) < 0) return false;
    return strcmp(child_buf, self_buf) != 0;
}
#endif

TEST_CASE("Integration: namespace isolation works (privileged)", "[integration][14.4][privileged]") {
#if defined(WIN32)
    SKIP("Namespace isolation tests run on Linux only.");
#else
    const std::string input_path = "integration_input_14_4.txt";
    const std::string output_path = "integration_output_14_4.txt";
    const scoped_cleanup cleanup{input_path, output_path};

    {
        std::ofstream in(input_path);
        in << "10 1";
    }

    pid_t pid = start_process(get_testapp_path(), input_path, output_path);
    REQUIRE(pid > 0);

    // Verify PID, mount, and network namespace isolation
    bool root = geteuid() == 0;
    if (root) {
        // Allow setup time for clone child to complete mount namespace operations
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        REQUIRE(namespace_differs("pid", pid));
        REQUIRE(namespace_differs("mnt", pid));
        REQUIRE(namespace_differs("net", pid));
    }

    // Wait for process to complete and reap it
    int status = 0;
    for (int i = 0; i < 100; ++i) {
        pid_t ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    REQUIRE(WIFEXITED(status));
    REQUIRE(WEXITSTATUS(status) == 0);

    REQUIRE(std::filesystem::exists(output_path));
    REQUIRE(std::filesystem::file_size(output_path) > 0);
#endif
}

TEST_CASE("Integration: memory limit enforced under namespace isolation (privileged)", "[integration][16.1][privileged]") {
#if defined(WIN32)
    SKIP("Linux privileged test.");
#else
    if (!can_run_cgroup_test()) {
        SKIP("Requires cgroup v2 memory controller with root privileges.");
    }

    const std::string input_path = "integration_input_16_1.txt";
    const std::string output_path = "integration_output_16_1.txt";
    const std::string log_path = "integration_log_16_1.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "50 131072"; // 50ms, 128 MB allocation
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 16ull * 1024ull * 1024ull, 2000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::MEMORY_LIMIT);
        REQUIRE(result.max_memory_used > 0);
    }
#endif
}

TEST_CASE("Integration: time limit enforced under namespace isolation (privileged)", "[integration][16.2][privileged]") {
#if defined(WIN32)
    SKIP("Linux privileged test.");
#else
    if (geteuid() != 0) {
        SKIP("Requires root privileges for namespace isolation.");
    }

    const std::string input_path = "integration_input_16_2.txt";
    const std::string output_path = "integration_output_16_2.txt";
    const std::string log_path = "integration_log_16_2.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "2000 1"; // 2000ms runtime, 1 KB allocation
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 512ull * 1024ull * 1024ull, 30),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::TIME_LIMIT);
        // time_used should be close to the configured limit (30ms)
        REQUIRE(result.time_used >= 20);
        REQUIRE(result.time_used < 2000);
    }
#endif
}

TEST_CASE("Integration: deep namespace isolation behavior (privileged)", "[integration][16.3][privileged]") {
#if defined(WIN32)
    SKIP("Linux privileged test.");
#else
    if (geteuid() != 0) {
        SKIP("Requires root privileges for namespace isolation.");
    }

    const std::string input_path = "integration_input_16_3.txt";
    const std::string output_path = "integration_output_16_3.txt";
    const scoped_cleanup cleanup{input_path, output_path};

    // Create a dummy input file (check_isolation ignores stdin)
    {
        std::ofstream in(input_path);
        in << "0 0";
    }

#ifdef CHECK_ISOLATION_PATH
    std::string helper = CHECK_ISOLATION_PATH;
#else
    std::string helper = "check_isolation";
#endif
    REQUIRE(std::filesystem::exists(helper));

    // Forward declaration of platform function
    pid_t pid = start_process(helper, input_path, output_path);
    REQUIRE(pid > 0);

    // Wait for it to complete
    bool exited = false;
    int status = 0;
    for (int i = 0; i < 100; ++i) {
        pid_t ret = waitpid(pid, &status, WNOHANG);
        if (ret == pid) {
            exited = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    REQUIRE(exited);
    REQUIRE(WIFEXITED(status));
    REQUIRE(WEXITSTATUS(status) == 0);

    REQUIRE(std::filesystem::file_size(output_path) > 0);
    const std::string content = read_file(output_path);

    // Parse key=value output
    auto get_val = [&](const std::string& key) -> std::string {
        std::string search = key + "=";
        auto pos = content.find(search);
        if (pos == std::string::npos) return "";
        auto start = pos + search.length();
        auto end = content.find('\n', start);
        return content.substr(start, end - start);
    };

    // Under clone + namespace isolation:
    //   UID/GID should be 65534 (nobody)
    //   PID 1 in the namespace should be the helper itself
    //   Network should have 0 interfaces (CLONE_NEWNET)
    REQUIRE(get_val("UID") == "65534");
    REQUIRE(get_val("GID") == "65534");

    // PID 1 comm should be the helper's name (truncated to 15 chars by kernel)
    std::string pid1_comm = get_val("PID1_COMM");
    REQUIRE_FALSE(pid1_comm.empty());
    REQUIRE(pid1_comm != "systemd");
    REQUIRE(pid1_comm != "init");

    // Number of network interfaces should be 0 in new net namespace
    std::string net_ifaces = get_val("NET_IFACES");
    REQUIRE(net_ifaces == "0");
#endif
}

TEST_CASE("Integration: output file has content on Linux", "[integration][11.3]") {
#if defined(WIN32)
    SKIP("Linux-specific test");
#endif

    const std::string input_path = "integration_input_11.3.txt";
    const std::string output_path = "integration_output_11.3.txt";
    const std::string log_path = "integration_log_11.3.log";
    const scoped_cleanup cleanup{input_path, output_path, log_path};

    {
        std::ofstream in(input_path);
        in << "10 1";
    }

    {
        mc::process_manager manager(
            make_cfg(input_path, output_path, 128ull * 1024ull * 1024ull, 1000),
            log_path);
        const mc::result_info result = manager.start_app();
        REQUIRE(result.status_code == mc::result_info::STATUS::OK);
    }

    REQUIRE(std::filesystem::file_size(output_path) > 0);
    const std::string content = read_file(output_path);
    REQUIRE(content.find("done after 10 ms.") != std::string::npos);
}
