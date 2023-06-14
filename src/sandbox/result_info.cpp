#include "result_info.hpp"

#include <sstream>

namespace mc {
    std::string result_info::JSON() const {
        std::stringstream ss;
        ss << "{";
        ss << "\"status_code\": \"" << this->status_code << "\", ";
        ss << "\"application\": \"" << this->config.application << "\", ";
        ss << "\"input\": \"" << this->config.input << "\"";
        ss << "\"output\": \"" << this->config.output << "\"";
        ss << "\"memory\": \"" << this->config.memory_limit << "\"";
        ss << "\"time\": \"" << this->config.time_limit << "\"";
        ss << "}";
        return ss.str();
    }
}