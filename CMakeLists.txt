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

# set(CMAKE_BINARY_DIR build)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_executable(sandbox ${SOURCE})
target_link_libraries(sandbox -static)
target_link_libraries(sandbox stdc++fs)

# testapp
add_executable(testapp examples/testapp/testapp.cpp)
