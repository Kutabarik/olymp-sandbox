#include <catch2/catch_test_macros.hpp>
#include "result_info.hpp"
#include <string>

TEST_CASE("ResultInfo: Default values", "[result_info]")
{
    mc::result_info result;
    REQUIRE(result.status_code == mc::result_info::STATUS::UNKNOWN);
    REQUIRE(result.max_memory_used == 0);
    REQUIRE(result.time_used == 0);
}

TEST_CASE("ResultInfo: Status codes", "[result_info]")
{
    mc::result_info result;

    result.status_code = mc::result_info::STATUS::OK;
    REQUIRE(result.status_code == mc::result_info::STATUS::OK);

    result.status_code = mc::result_info::STATUS::TIME_LIMIT;
    REQUIRE(result.status_code == mc::result_info::STATUS::TIME_LIMIT);

    result.status_code = mc::result_info::STATUS::MEMORY_LIMIT;
    REQUIRE(result.status_code == mc::result_info::STATUS::MEMORY_LIMIT);

    result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
    REQUIRE(result.status_code == mc::result_info::STATUS::RUNTIME_ERROR);
}

TEST_CASE("ResultInfo: JSON serialization", "[result_info]")
{
    mc::result_info result;
    result.status_code = mc::result_info::STATUS::OK;
    result.max_memory_used = 1024;
    result.time_used = 500;

    std::string json = result.JSON();
    REQUIRE(json.find("status_code") != std::string::npos);
    REQUIRE(json.find("application") != std::string::npos);
}