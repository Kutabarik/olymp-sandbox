cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER g++)

project(olymp-sandbox CXX)

set(TARGET windows)

set(SOURCE src/config.cpp
    src/definitions.cpp
    src/${TARGET}.cpp
    src/logger.cpp
    src/olymp-sandbox.cpp
    src/process_manager.cpp
    src/result_info.cpp)

set(CMAKE_BINARY_DIR build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(olymp-sandbox ${SOURCE})

# testapp
add_executable(testapp examples/testapp/testapp.cpp)
