#include "config.hpp"
#include "definitions.hpp"
#include "logger.hpp"

#include <getopt.h>

namespace mc
{
    bool config::init(int argc, char **argv)
    {
        mc::logger logger = mc::logger::STDOUT();

        int result;
        int option_index;
        while ((result = getopt_long(argc, argv, short_opts, long_opts, &option_index)) != -1)
        {
            switch (result)
            {
            case 'h':
                return false;
                break;
            case 'a':
                logger.info(std::string("app = ") + std::string(optarg));
                this->application = std::string(optarg);
                break;
            case 'm':
                logger.info(std::string("memory = ") + std::string(optarg));
                this->memory_limit = get_bytes(optarg);
                logger.info(std::string("memory converted = ") + std::to_string(this->memory_limit));
                break;
            case 't':
                logger.info(std::string("time = ") + std::string(optarg));
                this->time_limit = get_milliseconds(optarg);
                logger.info(std::string("time converted = ") + std::to_string(this->time_limit));
                break;
            case 'i':
                logger.info(std::string("input = ") + std::string(optarg));
                this->input = std::string(optarg);
                break;
            case 'o':
                logger.info(std::string("output = ") + std::string(optarg));
                this->output = std::string(optarg);
                break;
            default:
                break;
            }
        }
        return true;
    }

    config::config(const config &object) : application(object.application),
                                           memory_limit(object.memory_limit),
                                           time_limit(object.time_limit),
                                           input(object.input),
                                           output(object.output)
    {
    }

    config config::operator=(const config &object)
    {
        this->application = object.application;
        this->memory_limit = object.memory_limit;
        this->time_limit = object.time_limit;
        this->input = object.input;
        this->output = object.output;
        return *this;
    }
}