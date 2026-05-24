#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

namespace mc
{
    /**
     * @brief the structure defines a process manager configuration. 
     *        also used for result.
     * 
     */
    struct config
    {
        /**
         * @brief executed application
         * 
         */
        std::string application;
        /**
         * @brief input data file
         * 
         */
        std::string input = std::string("input.txt");
        /**
         * @brief output data file
         * 
         */
        std::string output = std::string("output.txt");
        /**
         * @brief memory limit for application or max memory, in bytes
         * 
         */
        size_t memory_limit = 8000000;
        /**
         * @brief time limit of application or execution time, in milliseconds
         * 
         */
        size_t time_limit = 1000;

        config() = default;
        config(const config& object);
        config operator = (const config& object);
        /**
         * @brief Initialize config from command-line options.
         *
         * @return true when initialization succeeds.
         * @return false when --help is requested or required arguments are missing/invalid.
         */
        bool init(int argc, char **argv);
    };
}

#endif
