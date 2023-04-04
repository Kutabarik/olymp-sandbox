#include "defs.h"
#include "process_manager.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv)
{
    uint64_t memory;
    uint16_t time;
    std::string application;
    std::string input = "in.txt";
    std::string output = "out.txt";

    int result;
    int option_index;

    if (argc == 1)
        print_usage(argv[0]);

    while ((result = getopt_long(argc, argv, short_opts, long_opts, &option_index)) != -1)
    {
        switch (result)
        {
        case 'h':
            print_usage(argv[0]);
            return 0;
            break;
        case 'a':
            application = optarg;
            break;
        case 'm':
            memory = get_bytes(optarg);
            break;
        case 't':
            time = get_milliseconds(optarg);
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        default:
            break;
        }
    }

    process_manager manager;
    process_manager::config config = {
        time,   // ms
        memory // bytes
    };

    std::cout << "[debug] config: { time: " << config.time_limit << 
        ", memory: " << config.memory_limit << " }" << std::endl;


    manager.set_config(config);

    process_manager::result_info info;

    info = manager.start_app(application, input, output);

    std::cout << "{ " << (int)(info.status_code) << " : " << info.status_message << " }" << std::endl;
    std::cout << "used memory: " << manager.used().memory_limit / 1024.0 << "KB" << std::endl;
    std::cout << "used time: " << manager.used().time_limit / 1000.0 << "S" << std::endl;
    return 0;
}
