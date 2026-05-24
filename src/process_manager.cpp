#include "definitions.hpp"
#include "process_manager.hpp"

#include <stdexcept>
#include <filesystem>

/**
 * @brief OS specific functions
 * 
 */
// ----------------------------------------------------------------
std::int64_t get_process_memory(process_id_t pid);
process_id_t start_process(
    const std::string& filename, 
    const std::string& input_file, 
    const std::string& output_file);
bool is_up_process(process_id_t pid);
bool stop_process(process_id_t pid);
// ----------------------------------------------------------------

namespace mc
{

    process_manager::process_manager(const mc::config &cfg, const std::string& log_path): 
        config(cfg),
        logger(log_path)
    {
        logger.info("Process manager initialized with configuration");
        logger.info("Application: " + config.application);
        logger.info("Input file: " + config.input);
        logger.info("Output file: " + config.output);
        logger.info("Memory limit: " + std::to_string(config.memory_limit) + " bytes");
        logger.info("Time limit: " + std::to_string(config.time_limit) + " ms");
        if (config.application.empty())
        {
            logger.error("Application path is empty");
            throw std::runtime_error("Application path is empty");
        }
        if (config.memory_limit == 0)
        {
            logger.error("Memory limit is zero");
            throw std::runtime_error("Memory limit is zero");
        }
        if (config.time_limit == 0)
        {
            logger.error("Time limit is zero");
            throw std::runtime_error("Time limit is zero");
        }
        if (config.input.empty())
        {
            logger.warn("Input file is empty, using default 'input.txt'");
            config.input = "input.txt";
        }
        if(std::filesystem::exists(config.input) == false)
        {
            logger.error("Input file does not exist: " + config.input);
            throw std::runtime_error("Input file does not exist: " + config.input);
        }
        if (std::filesystem::file_size(config.input) == 0)
        {
            logger.error("Input file is empty: " + config.input);
            throw std::runtime_error("Input file is empty: " + config.input);
        }
    }

    mc::result_info process_manager::start_app() const
    {
        logger.info(std::string("start guard process: ") + config.application);
        mc::result_info result;
        result.config = config;
        process_id_t pid = create_process();
        logger.info(std::string("child process id: ") + std::to_string((size_t)pid));
        if (pid == 0)
        {
            result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
            return result;
        }

        uint64_t start = get_current_time();
        uint64_t max_memory = 0;
        bool process_closed = false;

        while (is_process_up(pid))
        {
            if (is_time_limit(pid, start))
            {
                close_process(pid);
                process_closed = true;
                result.status_code = mc::result_info::STATUS::TIME_LIMIT;
                break;
            }
            int64_t tmp_mem = get_process_memory(pid);
            if (tmp_mem < 0) {
                logger.error("Failed to get process memory");
                result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
                break;
            }
            const uint64_t tmp_mem_u64 = static_cast<uint64_t>(tmp_mem);
            if (tmp_mem_u64 > max_memory)
                max_memory = tmp_mem_u64;
            if (tmp_mem_u64 > config.memory_limit)
            {
                close_process(pid);
                process_closed = true;
                result.status_code = mc::result_info::STATUS::MEMORY_LIMIT;
                break;
            }
        }

        bool exited_cleanly = false;
        if (!process_closed)
            exited_cleanly = close_process(pid);

        if (result.status_code == mc::result_info::STATUS::UNKNOWN)
        {
            if (!exited_cleanly)
            {
                result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
            }
            else if (!std::filesystem::exists(config.output))
            {
                logger.error("Output file does not exist: " + config.output);
                result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
            }
            else
            {
                result.status_code = mc::result_info::STATUS::OK;
            }
        }

        result.max_memory_used = max_memory;
        result.time_used = get_current_time() - start;
        if (result.status_code == mc::result_info::STATUS::OK)
            logger.info("end guard process");
        return result;
    }

    process_id_t process_manager::create_process() const
    {
        logger.info("Creating child process");
        return start_process(
            config.application,
            config.input,
            config.output
        );
    }

    bool process_manager::close_process(process_id_t pid) const
    {
        return ::stop_process(pid);
    }

    bool process_manager::is_memory_limit(process_id_t pid, uint64_t memory_limit) const
    {
        int64_t usedMemory = get_process_memory(pid);
        logger.info(std::string("used memory : ") + std::to_string(usedMemory));
        if (usedMemory < 0) {
            logger.error("Failed to get process memory");
            return false;
        }
        return static_cast<uint64_t>(usedMemory) > memory_limit;
    }

    bool process_manager::is_time_limit(process_id_t /*pid*/, uint64_t start_time) const
    {
        uint64_t current_time = get_current_time();
        logger.info(std::string("start time: ") + std::to_string(start_time));
        logger.info(std::string("curr time : ") + std::to_string(current_time));
        logger.info(std::string("time spent: ") + std::to_string(current_time - start_time));
        return current_time > config.time_limit + start_time;
    }

    bool process_manager::is_process_up(process_id_t pid) const
    {
        return is_up_process(pid);
    }

}
