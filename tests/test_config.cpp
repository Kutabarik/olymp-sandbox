#include <catch2/catch_test_macros.hpp>
#include "config.hpp"
#include <string>

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