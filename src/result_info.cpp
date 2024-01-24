#include "result_info.hpp"

#include <sstream>
#include <map>

namespace mc
{
    const std::map<result_info::STATUS, std::string> status = {
        {result_info::UNKNOWN, "unknown"},
        {result_info::OK, "ok"},
        {result_info::TIME_LIMIT, "time limit"},
        {result_info::MEMORY_LIMIT, "memory limit"},
        {result_info::RUNTIME_ERROR, "runtime error"}
    };

    std::ostream& operator << (std::ostream& out, result_info::STATUS p) {
        out << status.at(p);
        return out;
    }

    std::string result_info::JSON() const
    {
        std::stringstream ss;
        ss << "{"
            << "\"status_code\": \"" << this->status_code << "\", "
            << "\"application\": \"" << this->config.application << "\", "
            << "\"input\": \"" << this->config.input << "\","
            << "\"output\": \"" << this->config.output << "\","
            << "\"memory\": \"" << this->config.memory_limit << "\","
            << "\"time\": \"" << this->config.time_limit << "\""
            << "}";
        return ss.str();
    }
}