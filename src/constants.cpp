#include <getopt.h>
#include "definitions.h"

const char *short_opts = "a:i:o:h";
const struct option long_opts[] = {
    {"app", required_argument, 0, 'a'},
    {"memory", required_argument, 0, 'm'},
    {"time", required_argument, 0, 't'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}};
    