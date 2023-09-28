#include "logger.hpp"

#include <map>
#include <iostream>
#include <fstream>
#include <ctime>

namespace mc
{
    std::map<logger::MESSAGE_TYPE, std::string> LOG_MESSAGE = {
        {logger::MESSAGE_TYPE::_INFO, "info"},
        {logger::MESSAGE_TYPE::_DEBUG, "debug"},
        {logger::MESSAGE_TYPE::_WARNING, "warn"},
        {logger::MESSAGE_TYPE::_ERROR, "error"},
    };

    logger::logger(const std::string &filename) : out(new std::ofstream(filename.c_str()))
    {
    }

    void logger::write(const std::string &message, MESSAGE_TYPE type)
    {
        std::time_t time = std::time(nullptr);
        char timeString[32];
        std::strftime(timeString, 32, "%F %T", std::gmtime(&time));

        (*out) << timeString << " - [" << LOG_MESSAGE[type] << "]: " << message << std::endl;
    }

    void logger::info(const std::string &message)
    {
        write(message, MESSAGE_TYPE::_INFO);
    }

    void logger::debug(const std::string &message)
    {
        write(message, MESSAGE_TYPE::_DEBUG);
    }
    
    void logger::warn(const std::string &message)
    {
        write(message, MESSAGE_TYPE::_WARNING);
    }
    
    void logger::error(const std::string &message)
    {
        write(message, MESSAGE_TYPE::_ERROR);
    }

    logger logger::STDOUT()
    {
        logger object("");
        object.out = &(std::cout);
        return object;
    }

    logger logger::STDERR()
    {
        logger object("");
        object.out = &(std::cerr);
        return object;
    }

}