#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

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

/**
 * @brief functions
 *
 */

void print_usage(const std::string appname);
void run_task(std::string application, std::string input, std::string output);
void print_status(STATUS_TYPE status, double time, int memory);

#endif /* _DEFINITIONS_H_ */