#include <catch2/catch_test_macros.hpp>
#include "config.hpp"
#include "definitions.hpp"
#include <string>

namespace
{
    void reset_options()
    {
        options["app"] = {"", "path to application"};
        options["time"] = {"1s", "time limit"};
        options["memory"] = {"16M", "memory limit"};
        options["input"] = {"input.txt", "input file"};
        options["output"] = {"output.txt", "output file"};
        options["help"] = {"", "help message"};
    }
}

TEST_CASE("Config: Default values", "[config]")
{
    mc::config cfg;
    REQUIRE(cfg.application.empty());
    REQUIRE(cfg.input == "input.txt");
    REQUIRE(cfg.output == "output.txt");
    REQUIRE(cfg.memory_limit == 8000000);
    REQUIRE(cfg.time_limit == 1000);
}

TEST_CASE("Config: Copy constructor", "[config]")
{
    mc::config cfg1;
    cfg1.application = "/path/to/app";
    cfg1.memory_limit = 16000000;
    cfg1.time_limit = 2000;

    mc::config cfg2(cfg1);
    REQUIRE(cfg2.application == "/path/to/app");
    REQUIRE(cfg2.memory_limit == 16000000);
    REQUIRE(cfg2.time_limit == 2000);
}

TEST_CASE("Config: Assignment operator", "[config]")
{
    mc::config cfg1;
    cfg1.application = "/path/to/app";

    mc::config cfg2;
    cfg2 = cfg1;
    REQUIRE(cfg2.application == "/path/to/app");
}

// ============================================================
// SPRINT 2: Unit tests for config
// ============================================================

// 2.1: Parsing command-line arguments
TEST_CASE("Config: Parse --app argument", "[config][parse][2.1]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app"};
    int argc = 2;
    
    mc::config cfg;
    bool result = cfg.init(argc, (char**)argv);
    
    REQUIRE(result == true);
    REQUIRE(cfg.application == "/path/to/app");
}

TEST_CASE("Config: Parse --time argument", "[config][parse][2.1]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=2s"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 2000);  // 2s = 2000ms
}

TEST_CASE("Config: Parse --memory argument", "[config][parse][2.1]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=16m"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 16 * 1024 * 1024);
}

TEST_CASE("Config: Parse --input and --output arguments", "[config][parse][2.1]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", 
                          "--input=test/input.txt", "--output=test/output.txt"};
    int argc = 4;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.input == "test/input.txt");
    REQUIRE(cfg.output == "test/output.txt");
}

// 2.2: Parsing time suffixes (ms, s, m)
TEST_CASE("Config: Time suffix 'ms' (milliseconds)", "[config][time][2.2]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=500ms"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 500);
}

TEST_CASE("Config: Time suffix 's' (seconds)", "[config][time][2.2]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=3s"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 3000);
}

TEST_CASE("Config: Time suffix 'm' (minutes)", "[config][time][2.2]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=2m"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 2 * 60 * 1000);  // 2 * 60 * 1000 ms
}

TEST_CASE("Config: Time suffix uppercase 'S' (seconds)", "[config][time][2.2]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=1S"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 1000);
}

// 2.3: Parsing memory suffixes (b, k, m)
TEST_CASE("Config: Memory suffix 'b' (bytes)", "[config][memory][2.3]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=1024b"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 1024);
}

TEST_CASE("Config: Memory suffix 'k' (kilobytes)", "[config][memory][2.3]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=8k"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 8 * 1024);
}

TEST_CASE("Config: Memory suffix 'm' (megabytes)", "[config][memory][2.3]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=32m"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 32 * 1024 * 1024);
}

TEST_CASE("Config: Memory suffix uppercase 'K' (kilobytes)", "[config][memory][2.3]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=4K"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 4 * 1024);
}

// 2.4: Validation of mandatory arguments
TEST_CASE("Config: Missing --app argument returns false", "[config][validate][2.4]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--time=2s"};
    int argc = 2;
    
    mc::config cfg;
    bool result = cfg.init(argc, (char**)argv);
    
    REQUIRE(result == false);
}

TEST_CASE("Config: Help flag returns false", "[config][validate][2.4]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--help"};
    int argc = 2;
    
    mc::config cfg;
    bool result = cfg.init(argc, (char**)argv);
    
    REQUIRE(result == false);
}

TEST_CASE("Config: Valid --app argument returns true", "[config][validate][2.4]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app"};
    int argc = 2;
    
    mc::config cfg;
    bool result = cfg.init(argc, (char**)argv);
    
    REQUIRE(result == true);
}

TEST_CASE("Config: Empty --app value returns false", "[config][validate][2.4]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app="};
    int argc = 2;

    mc::config cfg;
    bool result = cfg.init(argc, (char**)argv);

    REQUIRE(result == false);
}

// 2.5: Safe parsing with boundaries and exception handling
TEST_CASE("Config: Large memory value parsing", "[config][safe][2.5]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=1024m"};
    int argc = 3;
    
    mc::config cfg;
    // Should not throw, should handle large values
    REQUIRE_NOTHROW(cfg.init(argc, (char**)argv));
    REQUIRE(cfg.memory_limit == 1024 * 1024 * 1024);
}

TEST_CASE("Config: Large time value parsing", "[config][safe][2.5]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=60m"};
    int argc = 3;
    
    mc::config cfg;
    // Should not throw, should handle large values
    REQUIRE_NOTHROW(cfg.init(argc, (char**)argv));
    REQUIRE(cfg.time_limit == 60 * 60 * 1000);  // 60 minutes in ms
}

TEST_CASE("Config: Memory with no suffix defaults to bytes", "[config][safe][2.5]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--memory=5000"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.memory_limit == 5000);  // No suffix = bytes
}

TEST_CASE("Config: Time with no suffix defaults to milliseconds", "[config][safe][2.5]")
{
    reset_options();

    const char* argv[] = {"sandbox", "--app=/path/to/app", "--time=1500"};
    int argc = 3;
    
    mc::config cfg;
    cfg.init(argc, (char**)argv);
    
    REQUIRE(cfg.time_limit == 1500);  // No suffix = ms
}