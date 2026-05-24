#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <memory>
#include <ostream>

namespace mc {

    /**
     * @brief Simple file-based logger.
     *
     * Writes timestamped log messages to a file.
     * Supports four log levels: debug, info, warning, error.
     */
    class logger {
    public:
        /** @brief Log message severity levels */
        enum class MESSAGE_TYPE {
            _INFO_ = 0,     /**< Normal operational messages */
            _DEBUG_,        /**< Detailed diagnostic output */
            _WARNING_,      /**< Recoverable issues */
            _ERROR_         /**< Non-recoverable issues */
        };

    private:
        std::shared_ptr<std::ostream> out;

    public:
        /**
         * @brief Open the log file for writing.
         *
         * Opens or creates the file at the given path.
         * Appends to the file if it already exists.
         *
         * @param filename Path to the log file
         */
        explicit logger(const std::string& filename);

        /**
         * @brief Write a message with the given log level.
         *
         * Format: "[level] message"
         *
         * @param message The log message text
         * @param type Severity level
         */
        void write(const std::string& message, MESSAGE_TYPE type) const;

        /** @brief Convenience: write with _INFO_ level */
        void info(const std::string& message) const;

        /** @brief Convenience: write with _DEBUG_ level */
        void debug(const std::string& message) const;

        /** @brief Convenience: write with _WARNING_ level */
        void warn(const std::string& message) const;

        /** @brief Convenience: write with _ERROR_ level */
        void error(const std::string& message) const;

        /**
         * @brief Create a logger writing to stdout.
         * @return logger instance
         */
        static logger STDOUT();

        /**
         * @brief Create a logger writing to stderr.
         * @return logger instance
         */
        static logger STDERR();
    };
}

#endif
