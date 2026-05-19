#include "result_info.hpp"

#include <sstream>
#include <map>

namespace
{
    std::string json_escape(const std::string& raw)
    {
        std::string escaped;
        escaped.reserve(raw.size() + 4);
        for (char ch : raw)
        {
            switch (ch)
            {
            case '"':  escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\b': escaped += "\\b";  break;
            case '\f': escaped += "\\f";  break;
            case '\n': escaped += "\\n";  break;
            case '\r': escaped += "\\r";  break;
            case '\t': escaped += "\\t";  break;
            default:   escaped += ch;     break;
            }
        }
        return escaped;
    }
}

namespace mc
{
    const std::map<result_info::STATUS, std::string> status = {
        {result_info::UNKNOWN, "unknown"},
        {result_info::OK, "ok"},
        {result_info::TIME_LIMIT, "time limit"},
        {result_info::MEMORY_LIMIT, "memory limit"},
        {result_info::RUNTIME_ERROR, "runtime error"}
    };

    std::ostream& operator << (std::ostream& out, result_info::STATUS p)
    {
        auto it = status.find(p);
        if (it != status.end())
            out << it->second;
        else
            out << "unknown";
        return out;
    }

    std::string result_info::JSON() const
    {
        std::stringstream ss;
        ss << "{"
            << "\"status_code\": \"" << this->status_code << "\", "
            << "\"application\": \"" << json_escape(this->config.application) << "\", "
            << "\"input\": \"" << json_escape(this->config.input) << "\", "
            << "\"output\": \"" << json_escape(this->config.output) << "\", "
            << "\"memory\": \"" << this->config.memory_limit << "\", "
            << "\"time\": \"" << this->config.time_limit << "\", "
            << "\"max_memory_used\": \"" << this->max_memory_used << "\", "
            << "\"time_used\": \"" << this->time_used << "\""
            << "}";
        return ss.str();
    }
}