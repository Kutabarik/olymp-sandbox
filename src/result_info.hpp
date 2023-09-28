#ifndef _RESULT_INFO_H_
#define _RESULT_INFO_H_

#include "config.hpp"

#include <string>
#include <cstdint>

namespace mc
{

    /**
     * @brief application execution result
     * 
     */
    struct result_info
    {
        enum STATUS : uint8_t
        {
            OK,
            TIME_LIMIT,
            MEMORY_LIMIT,
            RUNTIME_ERROR
        };

        STATUS status_code;
        mc::config config;

        std::string JSON() const;
    };
}

#endif