#include <catch2/catch_test_macros.hpp>
#include "process_manager.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

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

template <typename Callable>
std::string get_runtime_error_message(Callable&& callable) {
    try {
        callable();
    } catch (const std::runtime_error& error) {
        return error.what();
    }
    return std::string();
}

}  // namespace

std::int64_t get_process_memory(process_id_t) {
    return 1024;
}

process_id_t start_process(const std::string&, const std::string&, const std::string&) {
    return (process_id_t)(1);
}

bool is_up_process(process_id_t) {
    return g_is_up_calls++ == 0;
}

bool stop_process(process_id_t) {
    return true;
}

TEST_CASE("ProcessManager: constructor logs config once", "[process_manager][5.3]") {
    reset_process_stubs();
    const std::string log_path = "pm_test_log_53_once.log";
    const std::string input_path = "pm_test_input_53_once.txt";

    if (std::filesystem::exists(log_path)) {
        std::filesystem::remove(log_path);
    }
    {
        std::ofstream input(input_path);
        input << "1 2 3";
    }

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_process_manager.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 2000;

    mc::process_manager manager(cfg, log_path);
    (void)manager;

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);

    std::filesystem::remove(input_path);
    std::filesystem::remove(log_path);
}

TEST_CASE("ProcessManager: start_app does not duplicate init logs", "[process_manager][5.3]") {
    reset_process_stubs();
    const std::string log_path = "pm_test_log_53_start.log";
    const std::string input_path = "pm_test_input_53_start.txt";

    if (std::filesystem::exists(log_path)) {
        std::filesystem::remove(log_path);
    }
    {
        std::ofstream input(input_path);
        input << "1 2 3";
    }

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_process_manager.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 2000;

    mc::process_manager manager(cfg, log_path);
    (void)manager.start_app();

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);
    REQUIRE(content.find("Creating child process") != std::string::npos);

    std::filesystem::remove(input_path);
    std::filesystem::remove(log_path);
}

TEST_CASE("ProcessManager: empty input file throws", "[process_manager][6.1]") {
    reset_process_stubs();
    const std::string log_path = "pm_test_log_61.log";
    const std::string input_path = "pm_test_input_6.1.txt";

    if (std::filesystem::exists(log_path)) {
        std::filesystem::remove(log_path);
    }
    {
        std::ofstream f(input_path);
    }

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_6.1.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 2000;

        REQUIRE(get_runtime_error_message([&]() { mc::process_manager manager(cfg, log_path); })
            == "Input file is empty: " + input_path);

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Input file is empty: " + input_path) == 1);

    std::filesystem::remove(input_path);
    std::filesystem::remove(log_path);
}

TEST_CASE("ProcessManager: non-existent input file throws", "[process_manager][6.2]") {
    reset_process_stubs();
    const std::string input_path = "does_not_exist_6.2.txt";

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_6.2.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 2000;

        REQUIRE(get_runtime_error_message([&]() { mc::process_manager manager(cfg); })
            == "Input file does not exist: " + input_path);
}

TEST_CASE("ProcessManager: zero time limit throws", "[process_manager][6.3]") {
    reset_process_stubs();
    const std::string input_path = "pm_test_input_6.3.txt";
    {
        std::ofstream f(input_path);
        f << "test data";
    }

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_6.3.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 0;

        REQUIRE(get_runtime_error_message([&]() { mc::process_manager manager(cfg); })
            == "Time limit is zero");

    std::filesystem::remove(input_path);
}

TEST_CASE("ProcessManager: zero memory limit throws", "[process_manager][6.4]") {
    reset_process_stubs();
    const std::string input_path = "pm_test_input_6.4.txt";
    {
        std::ofstream f(input_path);
        f << "test data";
    }

    mc::config cfg;
    cfg.application = "fake_app.exe";
    cfg.input = input_path;
    cfg.output = "testfiles/output_6.4.txt";
    cfg.memory_limit = 0;
    cfg.time_limit = 2000;

        REQUIRE(get_runtime_error_message([&]() { mc::process_manager manager(cfg); })
            == "Memory limit is zero");

    std::filesystem::remove(input_path);
}

TEST_CASE("ProcessManager: empty application path throws", "[process_manager][6.5]") {
    reset_process_stubs();
    const std::string input_path = "pm_test_input_6.5.txt";
    {
        std::ofstream f(input_path);
        f << "test data";
    }

    mc::config cfg;
    cfg.application = "";
    cfg.input = input_path;
    cfg.output = "testfiles/output_6.5.txt";
    cfg.memory_limit = 16000000;
    cfg.time_limit = 2000;

        REQUIRE(get_runtime_error_message([&]() { mc::process_manager manager(cfg); })
            == "Application path is empty");

    std::filesystem::remove(input_path);
}
