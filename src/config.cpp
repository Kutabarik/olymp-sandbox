#include "config.hpp"
#include "definitions.hpp"
#include "logger.hpp"

extern keymap options;

namespace mc
{
    bool config::init(int argc, char **argv)
    {
        build_keys(options, argc, argv);

        if(options["help"].first.size())
        {
            return false;
        }

        this->application = options["app"].first;
        
        // Validate mandatory argument
        if(this->application.empty())
        {
            return false;
        }

        this->memory_limit = get_bytes(options["memory"].first);
        this->time_limit = get_milliseconds(options["time"].first);
        this->input = options["input"].first;
        this->output = options["output"].first;

        return true;
    }

    config::config(const config &object) :  application(object.application),
                                            input(object.input),
                                            output(object.output),
                                            memory_limit(object.memory_limit),
                                            time_limit(object.time_limit)
    {
    }

    config config::operator=(const config &object)
    {
        if (this == &object) return *this;
        this->application = object.application;
        this->memory_limit = object.memory_limit;
        this->time_limit = object.time_limit;
        this->input = object.input;
        this->output = object.output;
        return *this;
    }
}