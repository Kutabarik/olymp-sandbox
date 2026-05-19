#include <catch2/catch_test_macros.hpp>
#include "result_info.hpp"
#include <sstream>
#include <string>

// ============================================================
// 3.1: result_info structure fields
// ============================================================

TEST_CASE("ResultInfo: Default values", "[result_info][3.1]")
{
    mc::result_info result;
    REQUIRE(result.status_code == mc::result_info::STATUS::UNKNOWN);
    REQUIRE(result.max_memory_used == 0);
    REQUIRE(result.time_used == 0);
}

TEST_CASE("ResultInfo: Copy constructor", "[result_info][3.1]")
{
    mc::result_info r1;
    r1.status_code = mc::result_info::STATUS::OK;
    r1.max_memory_used = 4096;
    r1.time_used = 1500;

    mc::result_info r2(r1);
    REQUIRE(r2.status_code == mc::result_info::STATUS::OK);
    REQUIRE(r2.max_memory_used == 4096);
    REQUIRE(r2.time_used == 1500);
}

TEST_CASE("ResultInfo: Assignment operator", "[result_info][3.1]")
{
    mc::result_info r1;
    r1.status_code = mc::result_info::STATUS::TIME_LIMIT;
    r1.max_memory_used = 8192;

    mc::result_info r2;
    r2 = r1;
    REQUIRE(r2.status_code == mc::result_info::STATUS::TIME_LIMIT);
    REQUIRE(r2.max_memory_used == 8192);
}

// ============================================================
// 3.2: JSON serialization
// ============================================================

TEST_CASE("ResultInfo: JSON keys present", "[result_info][3.2]")
{
    mc::result_info result;
    std::string json = result.JSON();

    REQUIRE(json.find("\"status_code\"") != std::string::npos);
    REQUIRE(json.find("\"application\"") != std::string::npos);
    REQUIRE(json.find("\"input\"") != std::string::npos);
    REQUIRE(json.find("\"output\"") != std::string::npos);
    REQUIRE(json.find("\"memory\"") != std::string::npos);
    REQUIRE(json.find("\"time\"") != std::string::npos);
    REQUIRE(json.find("\"max_memory_used\"") != std::string::npos);
    REQUIRE(json.find("\"time_used\"") != std::string::npos);
}

TEST_CASE("ResultInfo: JSON values use key-value pairs", "[result_info][3.2]")
{
    mc::result_info result;
    result.status_code = mc::result_info::STATUS::OK;
    result.max_memory_used = 500;
    result.time_used = 500;

    std::string json = result.JSON();

    REQUIRE(json.find("\"max_memory_used\": \"500\"") != std::string::npos);
    REQUIRE(json.find("\"time_used\": \"500\"") != std::string::npos);
}

TEST_CASE("ResultInfo: JSON escapes special characters", "[result_info][3.2]")
{
    mc::result_info result;
    result.config.application = "C:\\tmp\\app.exe";
    result.config.input = "data\tfile.txt";
    result.config.output = "line1\nline2";

    std::string json = result.JSON();

    REQUIRE(json.find("C:\\\\tmp\\\\app.exe") != std::string::npos);
    REQUIRE(json.find("data\\tfile.txt") != std::string::npos);
    REQUIRE(json.find("line1\\nline2") != std::string::npos);
}

TEST_CASE("ResultInfo: JSON escapes double quotes", "[result_info][3.2]")
{
    mc::result_info result;
    result.config.application = "app\"name\".exe";

    std::string json = result.JSON();

    REQUIRE(json.find("app\\\"name\\\".exe") != std::string::npos);
}

TEST_CASE("ResultInfo: JSON format is brace-delimited", "[result_info][3.2]")
{
    mc::result_info result;
    std::string json = result.JSON();

    REQUIRE(json.front() == '{');
    REQUIRE(json.back() == '}');
}

// ============================================================
// 3.3: Status codes
// ============================================================

TEST_CASE("ResultInfo: Status codes are assignable", "[result_info][3.3]")
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

TEST_CASE("ResultInfo: operator<< produces correct strings", "[result_info][3.3]")
{
    std::stringstream ss;

    ss << mc::result_info::STATUS::UNKNOWN;
    REQUIRE(ss.str() == "unknown");
    ss.str("");

    ss << mc::result_info::STATUS::OK;
    REQUIRE(ss.str() == "ok");
    ss.str("");

    ss << mc::result_info::STATUS::TIME_LIMIT;
    REQUIRE(ss.str() == "time limit");
    ss.str("");

    ss << mc::result_info::STATUS::MEMORY_LIMIT;
    REQUIRE(ss.str() == "memory limit");
    ss.str("");

    ss << mc::result_info::STATUS::RUNTIME_ERROR;
    REQUIRE(ss.str() == "runtime error");
}

TEST_CASE("ResultInfo: operator<< handles unknown enum value", "[result_info][3.3]")
{
    std::stringstream ss;
    ss << static_cast<mc::result_info::STATUS>(99);
    REQUIRE(ss.str() == "unknown");
}

TEST_CASE("ResultInfo: All status codes are distinct", "[result_info][3.3]")
{
    REQUIRE(mc::result_info::STATUS::UNKNOWN != mc::result_info::STATUS::OK);
    REQUIRE(mc::result_info::STATUS::OK != mc::result_info::STATUS::TIME_LIMIT);
    REQUIRE(mc::result_info::STATUS::TIME_LIMIT != mc::result_info::STATUS::MEMORY_LIMIT);
    REQUIRE(mc::result_info::STATUS::MEMORY_LIMIT != mc::result_info::STATUS::RUNTIME_ERROR);
}