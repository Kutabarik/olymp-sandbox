#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

namespace mc
{
    /**
     * @brief Configuration parameters for process_manager.
     *
     * Holds all parameters for a single managed execution:
     * application path, I/O files, resource limits.
     * Also reused inside result_info to record what config was used.
     */
    struct config
    {
        /** @brief Path to the executable to be managed */
        std::string application;

        /** @brief Input file path (stdin will be redirected from this file) */
        std::string input = std::string("input.txt");

        /** @brief Output file path (stdout will be redirected to this file) */
        std::string output = std::string("output.txt");

        /** @brief Memory limit in bytes */
        size_t memory_limit = 8000000;

        /** @brief Time limit in milliseconds */
        size_t time_limit = 1000;

        config() = default;
        config(const config& object);

        config operator = (const config& object);

        /**
         * @brief Parse CLI arguments into this config.
         *
         * Expects arguments in the form --key=value.
         * Supported keys: app, time, memory, input, output, help.
         *
         * @param argc Argument count
         * @param argv Argument vector
         * @return true if parsing succeeded, false on --help
         */
        bool init(int argc, char **argv);
    };
}

#endif
