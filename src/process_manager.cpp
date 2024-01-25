#include "definitions.hpp"
#include "process_manager.hpp"


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

    process_manager::process_manager(const mc::config &cfg): 
        config(cfg),
        start_time(0),
        max_memory(0),
        logger(std::string("process_manager.log"))
    {
    }

    mc::result_info process_manager::start_app()
    {
        logger.info(std::string("start guard process: ") + config.application);
        mc::result_info result;
        result.config = config;
        // create child process
        process_id_t pid = create_process();
        logger.info(std::string("child process id: ") + std::to_string((size_t)pid));
        if (pid == 0)
        {
            result.status_code = mc::result_info::STATUS::RUNTIME_ERROR;
            return result;
        }

        // get current time and initial memory usage
        start_time = get_current_time();
        result.config.memory_limit = get_process_memory(pid);

        // while child process is active
        while (is_process_up(pid))
        {
            if (is_time_limit(pid))
            {
                close_process(pid);
                result.status_code = mc::result_info::STATUS::TIME_LIMIT;
                return result;
            }
            uint64_t tmp_mem = get_process_memory(pid);
            if (result.config.memory_limit < tmp_mem)
            {
                result.config.memory_limit = tmp_mem;
            }
            if (is_memory_limit(pid))
            {
                close_process(pid);
                result.status_code = mc::result_info::STATUS::MEMORY_LIMIT;
                return result;
            }
        }

        // get child process execution time
        result.config.time_limit = get_current_time() - start_time;

        // close_process(pid, mc::result_info::STATUS::OK);
        result.status_code = mc::result_info::STATUS::OK;
        logger.info("end guard process");
        return result;
    }

    process_id_t process_manager::create_process()
    {
        return start_process(config.application, config.input, config.output);
    }

    void process_manager::close_process(process_id_t pid) const
    {
        ::stop_process(pid);
    }

    bool process_manager::is_memory_limit(process_id_t pid)
    {
        int64_t usedMemory = get_process_memory(pid);
        logger.info(std::string("used memory : ") + std::to_string(usedMemory));
        return usedMemory < 0 && usedMemory > config.memory_limit;
    }

    bool process_manager::is_time_limit(process_id_t pid)
    {
        int64_t current_time = get_current_time();
        logger.info(std::string("time spent : ") + std::to_string(current_time - start_time));
        return current_time - start_time > config.time_limit;
    }

    bool process_manager::is_process_up(process_id_t pid) const
    {
        return is_up_process(pid);
    }

}
