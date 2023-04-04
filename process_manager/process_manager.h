#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <cstdint>
#include <string>
#include <chrono>

class process_manager
{
public:
    enum STATUS: uint8_t {
        OK,
        TIME_LIMIT,
        MEMORY_LIMIT
    };
    struct config
    {
        uint16_t time_limit = 1000;
        uint64_t memory_limit = 8000000;
    };
    struct result_info {
        uint8_t status_code;
        std::string status_message;
    };

private:
    config cfg;

    config maxed{0, 0};
    std::string managed_app;
    std::string input_file;
    std::string output_file;
    std::uint64_t start_time_ms;

public:
    void set_config(const process_manager::config &);
    process_manager::result_info start_app(
        const std::string& managed_app, 
        const std::string& input_file,
        const std::string& output_file);
    const config& used() const;
private:
    int64_t create_process(const std::string& command);
    bool is_memory_limit(int64_t pid);
    bool is_time_limit(int64_t pid);
    bool is_process_up(int64_t pid) const;
    void close_process(int64_t pid, uint16_t code) const;
};

#endif