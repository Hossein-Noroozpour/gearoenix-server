#include "slxs-log.hpp"

#include <cstdlib>
#include <iostream>

#ifdef DEBUG_MODE
std::mutex slxs::Log::file_guard;
#endif

std::ofstream slxs::Log::file;

void slxs::Log::initialize() noexcept
{
    file.open("slxs.log", std::ios::out | std::ios::ate);
    if (!file.is_open() || !file.good()) {
        std::cerr << "Error in opening of log file" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}