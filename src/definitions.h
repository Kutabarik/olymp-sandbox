#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#if defined(_WIN32)
    #define IS_WINDOWS
#else
#if defined(__linux__) || defined(__unix__) || defined(__unix)
    #define IS_LINUX
#else
    #error unknown operating system
#endif
#endif

#include <getopt.h>
#include <string>
#include <thread>

/**
 * @brief constants
 *
 */
extern const char *short_opts;
extern const option long_opts[];

enum class STATUS_TYPE
{
    SUCCESS = 0,
    FAIL,
    TIME_LIMIT,
    MEMORY_LIMIT,
    EXECUTION_FAILED
};

typedef std::size_t pid_type;

/**
 * @brief functions
 *
 */

void print_usage(const std::string appname);
void run_task(std::string application, std::string input, std::string output);
pid_type get_pid(std::string app_name);
int get_memory_usage(pid_type pid);
int get_time_usage(pid_type pid);
void print_status(STATUS_TYPE status);

#endif /* _DEFINITIONS_H_ */