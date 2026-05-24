#include <catch2/catch_test_macros.hpp>

#include "process_manager.hpp"

#include <filesystem>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <system_error>
#include <thread>

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
