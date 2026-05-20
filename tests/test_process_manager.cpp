#include <catch2/catch_test_macros.hpp>

#include "process_manager.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>

namespace {

int g_is_up_calls = 0;

void reset_process_stubs() {
    g_is_up_calls = 0;
}

size_t count_substring(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) {
        return 0;
    }
    size_t count = 0;
    size_t pos = 0;
    while ((pos = haystack.find(needle, pos)) != std::string::npos) {
        ++count;
        pos += needle.size();
    }
    return count;
}

std::string read_all(const std::string& path) {
    std::ifstream in(path);
    return std::string((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
}

/**
 * @brief RAII fixture that creates a unique temp directory and sets it as the
 *        current working directory for the duration of a test, ensuring each
 *        test case is isolated (process_manager.log, input, and output files
 *        are created inside the per-test temp dir).
 */
struct ProcessManagerFixture {
    std::filesystem::path orig_cwd;
    std::filesystem::path test_dir;

    ProcessManagerFixture() : orig_cwd(std::filesystem::current_path()) {
        test_dir = std::filesystem::temp_directory_path() /
                   ("pm_test_" + std::to_string(::getpid()));
        std::filesystem::create_directories(test_dir);
        std::filesystem::current_path(test_dir);
    }

    ~ProcessManagerFixture() {
        std::filesystem::current_path(orig_cwd);
        std::filesystem::remove_all(test_dir);
    }
};

}  // namespace

std::int64_t get_process_memory(process_id_t) {
    return 1024;
}

process_id_t start_process(const std::string&, const std::string&, const std::string&) {
    return static_cast<process_id_t>(1);
}

bool is_up_process(process_id_t) {
    return g_is_up_calls++ == 0;
}

bool stop_process(process_id_t) {
    return true;
}

TEST_CASE_METHOD(ProcessManagerFixture,
                 "ProcessManager: constructor logs config once",
                 "[process_manager][5.3]") {
    reset_process_stubs();

    const std::string log_path   = (test_dir / "process_manager.log").string();
    const std::string input_path = (test_dir / "pm_test_input.txt").string();

    {
        std::ofstream input(input_path);
        input << "1 2 3";
    }

    mc::config cfg;
    cfg.application  = "fake_app.exe";
    cfg.input        = input_path;
    cfg.output       = (test_dir / "output.txt").string();
    cfg.memory_limit = 16000000;
    cfg.time_limit   = 2000;

    mc::process_manager manager(cfg);
    (void)manager;

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);
}

TEST_CASE_METHOD(ProcessManagerFixture,
                 "ProcessManager: start_app does not duplicate init logs",
                 "[process_manager][5.3]") {
    reset_process_stubs();

    const std::string log_path   = (test_dir / "process_manager.log").string();
    const std::string input_path = (test_dir / "pm_test_input.txt").string();

    {
        std::ofstream input(input_path);
        input << "1 2 3";
    }

    mc::config cfg;
    cfg.application  = "fake_app.exe";
    cfg.input        = input_path;
    cfg.output       = (test_dir / "output.txt").string();
    cfg.memory_limit = 16000000;
    cfg.time_limit   = 2000;

    mc::process_manager manager(cfg);
    (void)manager.start_app();

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);
    REQUIRE(content.find("Creating child process") != std::string::npos);
}
