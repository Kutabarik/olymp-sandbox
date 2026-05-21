#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <chrono>

#include "definitions.hpp"

keymap options = {
    {"app", {"", "path to application"}},
    {"time", {"1s", "time limit"}},
    {"memory", {"16M", "memory limit"}},
    {"input", {"input.txt", "input file"}},
    {"output", {"output.txt", "output file"}},
    {"help", {"", "help message"}}
};

void print_usage(const std::string &appname)
{

    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help" << std::endl;
    std::cout << "\t" << appname << " --app=<app> [--time=<time>] [--memory=<memory>] [--input=<in.file>] [--output=<outfile>]" << std::endl;
    std::cout << "key description:" << std::endl;
    print_options(options);
}

std::string get_measure(const std::string &value)
{
    size_t index = value.size() - 1;
    while (!isdigit(value.at(index)))
    {
        --index;
    }
    if (index == value.size() - 1)
    {
        return std::string();
    }
    return value.substr(index + 1);
}

size_t get_bytes(const std::string &memory)
{
    std::map<std::string, std::size_t> table = {
        {std::string("b"), 1ul},
        {std::string("B"), 1ul},
        {std::string("k"), 1024ul},
        {std::string("K"), 1024ul},
        {std::string("m"), 1024ul * 1024ul},
        {std::string("M"), 1024ul * 1024ul}};
    size_t value = std::stoll(memory);
    std::string measure = get_measure(memory);
    if (table.find(measure) == table.end())
    {
        measure = std::string("b");
    }
    return value * table[measure];
}

size_t get_milliseconds(const std::string &time)
{
    std::map<std::string, std::size_t> table = {
        {std::string("ms"), 1ul},
        {std::string("MS"), 1ul},
        {std::string("s"), 1000ul},
        {std::string("m"), 60ul * 1000ul},
        {std::string("S"), 1000ul},
        {std::string("M"), 60ul * 1000ul}};
    size_t value = std::stoll(time);
    std::string measure = get_measure(time);
    if (table.find(measure) == table.end())
    {
        measure = std::string("ms");
    }
    return value * table[measure];
}

uint64_t get_current_time()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

/**
 * @brief
 *
 * @param keys
 * @param argc
 * @param argv
 */

void build_keys(keymap &keys, int argc, char **argv)
{
    std::string key, value, aux;
    for (int i = 1; i < argc; ++i)
    {
        aux = std::string(argv[i]);
        if (aux.rfind("--", 0) != 0)
        {
            continue;
        }

        const std::string::size_type eq_pos = aux.find('=');
        if (eq_pos == std::string::npos)
        {
            key = aux.substr(2);
            if (keys.count(key) == 0)
            {
                continue;
            }
            keys[key].first = key;
            continue;
        }

        key = aux.substr(2, eq_pos - 2);
        value = aux.substr(eq_pos + 1);
        if (keys.count(key) == 0)
        {
            continue;
        }
        keys[key].first = value;
    }
}

void print_options(const keymap &options)
{
    for (auto option : options)
    {
        std::cout << "\t" + option.first << "\t - " << option.second.second
            << ", default value " 
            << ((option.second.first.size()) ? option.second.first : "not set") << std::endl;
    }
}
