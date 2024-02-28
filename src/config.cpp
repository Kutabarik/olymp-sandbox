#include "config.hpp"
#include "definitions.hpp"
#include "logger.hpp"

extern keymap options;

namespace mc
{
    bool config::init(int argc, char **argv)
    {
        mc::logger logger = mc::logger::STDOUT();


        build_keys(options, argc, argv);

        this->application = options["app"].first;
        this->memory_limit = get_bytes(options["memory"].first);
        this->time_limit = get_milliseconds(options["time"].first);
        this->input = options["input"].first;
        this->output = options["output"].first;

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