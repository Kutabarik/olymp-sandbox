#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <getopt.h>
#include <string>

/**
 * @brief constants
 * 
 */
extern const char *short_opts;
extern const option long_opts[];

/**
 * @brief functions
 * 
 */

void print_usage(const std::string appname);
void run_task(std::string application, std::string input, std::string output);

#endif /* _DEFINITIONS_H_ */