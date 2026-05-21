#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <memory>
#include <ostream>

namespace mc {
    class logger {
    public:
        enum class MESSAGE_TYPE {
            _INFO_ = 0,
            _DEBUG_,
            _WARNING_,
            _ERROR_
        };
    private:
        std::shared_ptr<std::ostream> out;
    public:
        logger(const std::string& filename);
        void write(const std::string& message, MESSAGE_TYPE type) const;
        void info(const std::string& message) const;
        void debug(const std::string& message) const;
        void warn(const std::string& message) const;
        void error(const std::string& message) const;

        static logger STDOUT();
        static logger STDERR();
    };
}

#endif