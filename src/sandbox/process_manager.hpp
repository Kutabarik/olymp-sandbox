#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "definitions.hpp"
#include "config.hpp"
#include "result_info.hpp"
#include "logger.hpp"

#include <string>

namespace mc
{
    class process_manager
    {
    private:
        mc::config config;

        size_t start_time;
        size_t max_memory;
        mc::logger logger;

    public:
        process_manager(const mc::config &);
        mc::result_info start_app();

    private:
        process_id_t create_process();
        bool is_memory_limit(process_id_t pid);
        bool is_time_limit(process_id_t pid);
        bool is_process_up(process_id_t pid) const;
        void close_process(process_id_t pid) const;
    };
}

#endif