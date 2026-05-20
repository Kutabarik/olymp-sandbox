#include <catch2/catch_test_macros.hpp>

#include "process_manager.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#include <type_traits>

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

}  // namespace

std::int64_t get_process_memory(process_id_t) {
    return 1024;
}

process_id_t start_process(const std::string&, const std::string&, const std::string&) {
    if constexpr (std::is_pointer_v<process_id_t>) {
        return reinterpret_cast<process_id_t>(1);
    } else {
        return static_cast<process_id_t>(1);
    }
}

bool is_up_process(process_id_t) {
    return g_is_up_calls++ == 0;
}

bool stop_process(process_id_t) {
    return true;
}

TEST_CASE("ProcessManager: constructor logs config once", "[process_manager][5.3]") {
    reset_process_stubs();
    const std::string log_path = "process_manager.log";
    const std::string input_path = "pm_test_input.txt";

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

    mc::process_manager manager(cfg);
    (void)manager;

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);

    std::filesystem::remove(input_path);
}

TEST_CASE("ProcessManager: start_app does not duplicate init logs", "[process_manager][5.3]") {
    reset_process_stubs();
    const std::string log_path = "process_manager.log";
    const std::string input_path = "pm_test_input.txt";

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

    mc::process_manager manager(cfg);
    (void)manager.start_app();

    const std::string content = read_all(log_path);
    REQUIRE(count_substring(content, "Process manager initialized with configuration") == 1);
    REQUIRE(content.find("Creating child process") != std::string::npos);

    std::filesystem::remove(input_path);
}
