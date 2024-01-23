#include <iostream>
#include <map>
#include <string>
#include <cstdlib>
#include <chrono>

#include "definitions.hpp"

const char *short_opts = "a:m:t:i:o:h";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"memory", required_argument, 0, 'm'},
    {"time", required_argument, 0, 't'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

void print_usage(const std::string& appname)
{

    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
    std::cout << "key description:" << std::endl;
    std::cout << "\ta | app\t\t- name of managed application" << std::endl;
    std::cout << "\tt | time\t- max execution time, milliseconds (ms, default) or seconds (s) or minutes (m)" << std::endl;
    std::cout << "\tm | memory\t- max memory usage, bytes (b, default), kilobytes (k) or megabytes (m)" << std::endl;
    std::cout << "\ti | input\t- input data file for application" << std::endl;
    std::cout << "\to | output\t- output data file for application" << std::endl;
}

std::string get_measure(const std::string& value) {
    size_t index = value.size() - 1;
    while(!isdigit(value.at(index))) {
        --index;
    }
    if(index == value.size() - 1) {
        return std::string();
    }
    return value.substr(index + 1);
}

size_t get_bytes(const std::string& memory) {
    std::map<std::string, std::size_t> table = {
        {std::string("b"), 1ul},
        {std::string("B"), 1ul},
        {std::string("k"), 1024ul},
        {std::string("K"), 1024ul},
        {std::string("m"), 1024ul*1024ul},
        {std::string("M"), 1024ul*1024ul}
    };
    size_t value = std::stoll(memory);
    std::string measure = get_measure(memory);
    if(table.find(measure) == table.end()) {
        measure = std::string("b");
    }
    return value * table[measure];
}

size_t get_milliseconds(const std::string& time) {
    std::map<std::string, std::size_t> table = {
        {std::string("ms"), 1ul},
        {std::string("MS"), 1ul},
        {std::string("s"), 1000ul},
        {std::string("m"), 60ul*1000ul},
        {std::string("S"), 1000ul},
        {std::string("M"), 60ul*1000ul}
    };
    size_t value = std::stoll(time);
    std::string measure = get_measure(time);
    if(table.find(measure) == table.end()) {
        measure = std::string("ms");
    }
    return value * table[measure];
}

uint64_t get_current_time()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}