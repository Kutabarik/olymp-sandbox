#include <getopt.h>
#include <iostream>
#include <string>

void print_usage(const std::string appname);

const char *short_opts = "a:i:o:h";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};

int main(int argc, char **argv)
{
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

    std::cout << application << " < " << input << " > " << output << std::endl;
    return 0;
}

void print_usage(const std::string appname)
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\t" << appname << " --help | -h" << std::endl;
    std::cout << "\t" << appname << " -a <app> -i <in.file> -o <outfile>" << std::endl;
    std::cout << "\t" << appname << " --app=<app> --input=<in.file> --output=<outfile>" << std::endl;
}