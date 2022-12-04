#include <getopt.h>
#include <iostream>
#include <string>

void print_usage(const std::string appname);

const char *short_opts = "a:i:o:h";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"memory", required_argument, 0, 'm'},
    {"time", required_argument, 0, 't'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

int main(int argc, char **argv)
{
    int memory;
    double time;
    std::string application;
    std::string input = "";
    std::string output = "out.txt";

    int result;
    int option_index;

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
            std::cout << application << std::endl;
            break;
        case 'm':
            memory = atoi(optarg);
            std::cout << memory << std::endl;
        break;
        case 't':
            time = atoi(optarg);
            std::cout << time << std::endl;
            break;
        case 'i':
            input = optarg;
            std::cout << input << std::endl;
            break;
        case 'o':
            output = optarg;
            std::cout << output << std::endl;
            break;
        default:
            break;
        }
    }

    return 0;
}

void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -t <time> -m <memory> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --time=<time> --memory=<memory> --input=<in.file> --output=<outfile>" << std::endl;
}