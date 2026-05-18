#include <catch2/catch_test_macros.hpp>
#include "definitions.hpp"
#include <cstddef>

TEST_CASE("get_bytes: Memory suffixes", "[definitions][parse]")
{
    REQUIRE(get_bytes("100b") == 100);
    REQUIRE(get_bytes("100B") == 100);
    REQUIRE(get_bytes("1k") == 1024);
    REQUIRE(get_bytes("1K") == 1024);
    REQUIRE(get_bytes("1m") == 1024 * 1024);
    REQUIRE(get_bytes("1M") == 1024 * 1024);
}

TEST_CASE("get_bytes: Without suffix defaults to bytes", "[definitions][parse]")
{
    REQUIRE(get_bytes("100") == 100);
}

TEST_CASE("get_milliseconds: Time suffixes", "[definitions][parse]")
{
    REQUIRE(get_milliseconds("100ms") == 100);
    REQUIRE(get_milliseconds("100MS") == 100);
    REQUIRE(get_milliseconds("1s") == 1000);
    REQUIRE(get_milliseconds("1S") == 1000);
    REQUIRE(get_milliseconds("1m") == 60000);
    REQUIRE(get_milliseconds("1M") == 60000);
}

TEST_CASE("get_milliseconds: Without suffix defaults to ms", "[definitions][parse]")
{
    REQUIRE(get_milliseconds("100") == 100);
}

TEST_CASE("get_current_time: Returns positive value", "[definitions]")
{
    uint64_t t = get_current_time();
    REQUIRE(t > 0);
}