#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

#include "defs.h"

const char *short_opts = "a:i:o:h:t:m";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"memory", required_argument, 0, 'm'},
    {"time", required_argument, 0, 't'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
}

std::string get_measure(std::string value) {
    size_t index = value.size() - 1;
    while(!isdigit(value[index])) {
        --index;
    }

    return value.substr(index + 1);
}

size_t get_bytes(std::string memory) {
    std::map<std::string, std::size_t> table = {
        {"b", 1ll},
        {"B", 1ll},
        {"k", 1024ll},
        {"K", 1024ll},
        {"m", 1024ll*1024ll},
        {"M", 1024ll*1024ll}
    };
    size_t value = std::stoll(memory);
    std::string measure = get_measure(memory);
    if(table.find(measure) == table.end()) {
        measure = "b";
    }
    return value * table[measure];
}

size_t get_milliseconds(std::string time) {
    std::map<std::string, std::size_t> table = {
        {"ms", 1ll},
        {"MS", 1ll},
        {"s", 1000ll},
        {"m", 60ll*1000ll},
        {"S", 1000ll},
        {"M", 60ll*1000ll}
    };
    size_t value = std::stoll(time);
    std::string measure = get_measure(time);
    if(table.find(measure) == table.end()) {
        measure = "ms";
    }
    return value * table[measure];
}