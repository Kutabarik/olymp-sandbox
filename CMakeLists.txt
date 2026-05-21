cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER g++)

project(olymp-sandbox CXX)

# detect OS and set macros WIN32 or linux
INCLUDE (CheckIncludeFiles)
CHECK_INCLUDE_FILES(windows.h WIN32)
if(WIN32)
   add_definitions(-DWIN32)
   set(TARGET windows)
else()
   add_definitions(-Dlinux)
   set(TARGET linux)
endif(WIN32)

set(SOURCE src/config.cpp
    src/definitions.cpp
    src/${TARGET}.cpp
    src/logger.cpp
    src/olymp-sandbox.cpp
    src/process_manager.cpp
    src/result_info.cpp)

add_executable(sandbox ${SOURCE})
target_link_libraries(sandbox -static)
target_link_libraries(sandbox stdc++fs)

# testapp
add_executable(testapp examples/testapp/testapp.cpp)

# ============================================================
# Testing with Catch2
# ============================================================
include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(catch2)

include(Catch)

enable_testing()

# Test sources
set(TEST_SOURCES
    tests/test_config.cpp
    tests/test_definitions.cpp
    tests/test_result_info.cpp
    tests/test_process_manager.cpp
)

# Source files needed for tests
set(TEST_LIB_SOURCES
    src/config.cpp
    src/definitions.cpp
    src/logger.cpp
    src/process_manager.cpp
    src/result_info.cpp
)

add_executable(run_tests ${TEST_SOURCES} ${TEST_LIB_SOURCES})
target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(run_tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(run_tests PRIVATE -Wall -Wextra -Wpedantic)

# Register tests
catch_discover_tests(run_tests)

# Integration tests (Sprint 9)
set(INTEGRATION_TEST_SOURCES
    tests/test_integration.cpp
)

set(INTEGRATION_LIB_SOURCES
    src/config.cpp
    src/definitions.cpp
    src/logger.cpp
    src/process_manager.cpp
    src/result_info.cpp
    src/${TARGET}.cpp
)

add_executable(run_integration_tests ${INTEGRATION_TEST_SOURCES} ${INTEGRATION_LIB_SOURCES})
target_include_directories(run_integration_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(run_integration_tests PRIVATE Catch2::Catch2WithMain)
target_compile_options(run_integration_tests PRIVATE -Wall -Wextra -Wpedantic)

catch_discover_tests(run_integration_tests)
