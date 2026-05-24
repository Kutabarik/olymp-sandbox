#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "definitions.hpp"
#include "config.hpp"
#include "result_info.hpp"
#include "logger.hpp"

#include <string>

namespace mc
{
    /**
     * @brief Manages the lifecycle of a single child process.
     *
     * Validates configuration on construction, spawns a child process
     * on start_app(), monitors its resource usage (memory/time),
     * enforces limits, and collects execution results.
     */
    class process_manager
    {
    private:
        mc::config config;
        mc::logger logger;

    public:
        /**
         * @brief Construct a process_manager with the given config.
         *
         * Validates the config: application path must be non-empty,
         * limits must be non-zero, input file must exist and be non-empty.
         * On validation failure, throws std::runtime_error.
         *
         * @param cfg Configuration to use
         * @param log_path File path for the log (default: "process_manager.log")
         */
        process_manager(const mc::config &cfg, const std::string& log_path = "process_manager.log");

        /**
         * @brief Start the managed application and wait for it to complete.
         *
         * Creates a child process, monitors its memory and time usage,
         * enforces configured limits, validates output file existence.
         *
         * @return result_info with status code, peak memory, and runtime
         */
        mc::result_info start_app() const;

    private:
        /** @brief Delegate to platform-specific start_process() */
        process_id_t create_process() const;

        /**
         * @brief Check if process exceeds memory limit.
         * @return true if usedMemory > memory_limit
         */
        bool is_memory_limit(process_id_t pid, uint64_t memory_limit) const;

        /**
         * @brief Check if process exceeds time limit.
         * @return true if (now - start_time) > config.time_limit
         */
        bool is_time_limit(process_id_t pid, uint64_t start_time) const;

        /** @brief Delegate to platform-specific is_up_process() */
        bool is_process_up(process_id_t pid) const;

        /**
         * @brief Stop the child process and release resources.
         * @return true if the process was stopped successfully
         */
        bool close_process(process_id_t pid) const;
    };
}

#endif
