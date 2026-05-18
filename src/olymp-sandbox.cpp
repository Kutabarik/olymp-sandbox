#include "definitions.hpp"
#include "config.hpp"
#include "result_info.hpp"
#include "process_manager.hpp"

#include <iostream>
#include <filesystem>

int main(int argc, char **argv)
{
    mc::logger logger = mc::logger::STDOUT();
    std::string appname { std::filesystem::path(argv[0]).filename().generic_string()};
    logger.info(std::string("start ") + appname + " ...");
    mc::config config;

    if (argc == 1 || !config.init(argc, argv))
    {
        logger.error("Invalid arguments or failed to initialize config.");
        print_usage(appname);
        return 1;
    }
    logger.info(std::string(" ---------------------------------------- "));
    logger.info(std::string("config.application: " + config.application));
    logger.info(std::string("config.time: ") + std::to_string(config.time_limit));
    logger.info(std::string("config.memory: ") + std::to_string(config.memory_limit));
    logger.info(std::string("config.input: " + config.input));
    logger.info(std::string("config.output: " + config.output));
    logger.info(std::string("start job"));
    logger.info(std::string(" ---------------------------------------- "));

    // check if application exists
    if (!std::filesystem::exists(config.application))
    {
        logger.error("Application file does not exist: " + config.application);
        return 1;
    }
    // check if input file exists
    if (!std::filesystem::exists(config.input))
    {
        logger.error("Input file does not exist: " + config.input);
        return 1;
    }
    int exit_code = 0;
    try
    {
        mc::process_manager manager(config);
        logger.info("start app");
        mc::result_info result = manager.start_app();

        std::cout << result.JSON() << std::endl;
        logger.info("end app");
    }
    catch (const std::exception &e)
    {
        logger.error(std::string("Exception: ") + e.what());
        exit_code = 2;
    }
    logger.info("end job");
    logger.info("Current directory: " + std::filesystem::current_path().string());
    logger.info("Absolute input path: " + std::filesystem::absolute(config.input).string());
    return exit_code;
}
