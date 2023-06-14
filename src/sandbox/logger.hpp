#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>

namespace mc {
    class logger {
    public:
        enum MESSAGE_TYPE {
            _INFO = 0,
            _DEBUG,
            _WARNING,
            _ERROR,
        };
    private:
        std::ostream* out;
    public:
        logger(const std::string& filename);
        void write(const std::string& message, MESSAGE_TYPE type);
        void info(const std::string& message);
        void debug(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);

        static logger STDOUT();
        static logger STDERR();
    };
}

#endif