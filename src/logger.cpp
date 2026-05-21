#include "logger.hpp"

#include <map>
#include <iostream>
#include <fstream>
#include <ctime>

namespace mc
{
    std::map<logger::MESSAGE_TYPE, std::string> LOG_MESSAGE = {
        {logger::MESSAGE_TYPE::_INFO_, "info"},
        {logger::MESSAGE_TYPE::_DEBUG_, "debug"},
        {logger::MESSAGE_TYPE::_WARNING_, "warn"},
        {logger::MESSAGE_TYPE::_ERROR_, "error"},
    };

    logger::logger(const std::string &filename)
    {
        if (filename.empty())
        {
            out = std::shared_ptr<std::ostream>(&std::cout, [](std::ostream*) {});
        }
        else
        {
            out = std::make_shared<std::ofstream>(filename.c_str());
        }
    }

    void logger::write(const std::string &message, MESSAGE_TYPE type) const
    {
        std::time_t time = std::time(nullptr);
        char timeString[32];
        std::strftime(timeString, 32, "%F %T", std::gmtime(&time));

        (*out) << timeString << " - [" << LOG_MESSAGE[type] << "]: " << message << std::endl;
    }

    void logger::info(const std::string &message) const
    {
        write(message, MESSAGE_TYPE::_INFO_);
    }

    void logger::debug(const std::string &message) const
    {
        write(message, MESSAGE_TYPE::_DEBUG_);
    }
    
    void logger::warn(const std::string &message) const
    {
        write(message, MESSAGE_TYPE::_WARNING_);
    }
    
    void logger::error(const std::string &message) const
    {
        write(message, MESSAGE_TYPE::_ERROR_);
    }

    logger logger::STDOUT()
    {
        return logger("");
    }

    logger logger::STDERR()
    {
        logger object("");
        object.out = std::shared_ptr<std::ostream>(&std::cerr, [](std::ostream*) {});
        return object;
    }

}