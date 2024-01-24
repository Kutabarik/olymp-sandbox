#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <getopt.h>
#include <string>
#include <cstdint>

/**
 * @brief types
 * 
 */

// #define WIN32
#ifndef linux
    #define linux
#endif

#if defined(WIN32)
    #include <Windows.h>
    typedef HANDLE process_id_t;
#elif defined(linux)
    typedef pid_t process_id_t;
#endif

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

void print_usage(const std::string& appname);
std::string get_measure(const std::string& value);
size_t get_bytes(const std::string& memory);
size_t get_milliseconds(const std::string& time);
uint64_t get_current_time();


#endif /* _DEFINITIONS_H_ */