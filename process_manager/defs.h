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
std::string get_measure(std::string);
size_t get_bytes(std::string memory);
size_t get_milliseconds(std::string time);

#endif /* _DEFINITIONS_H_ */