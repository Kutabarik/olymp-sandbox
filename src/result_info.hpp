#ifndef _RESULT_INFO_H_
#define _RESULT_INFO_H_

#include "config.hpp"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>

namespace mc
{
    /**
     * @brief Execution result returned by process_manager::start_app().
     *
     * Contains the outcome status, the config that was used,
     * peak memory consumption, and wall-clock runtime.
     */
    struct result_info
    {
        /**
         * @brief Execution outcome codes.
         */
        enum STATUS : uint8_t
        {
            UNKNOWN,        /**< Initial state, not yet executed */
            OK,             /**< Completed within all limits */
            TIME_LIMIT,     /**< Killed for exceeding time limit */
            MEMORY_LIMIT,   /**< Killed for exceeding memory limit */
            RUNTIME_ERROR   /**< Failed to start or abnormal exit */
        };

        /** @brief Outcome of the execution */
        STATUS status_code = STATUS::UNKNOWN;

        /** @brief Configuration used for this execution */
        mc::config config;

        /** @brief Peak memory consumption in bytes */
        size_t max_memory_used = 0;

        /** @brief Wall-clock runtime in milliseconds */
        int64_t time_used = 0;

        /**
         * @brief Serialize execution result as JSON.
         *
         * Includes status_code, config fields, max_memory_used, and time_used.
         * Escapes special characters in string fields.
         *
         * @return std::string containing the JSON document
         */
        std::string JSON() const;
    };

    /**
     * @brief Stream operator for STATUS enum.
     *
     * Outputs human-readable status name.
     * Falls back to "unknown" for unrecognized values.
     */
    std::ostream& operator<<(std::ostream& out, result_info::STATUS p);
}

#endif
