#ifndef SLXS_LOG_HPP
#define SLXS_LOG_HPP

#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <map>
#include <mutex>
#include <sstream>
#include <thread>

#define DEBUG_MODE

#define CONCAT_0(x, y) x##y
#define CONCAT_1(x, y) CONCAT_0(x, y)
#define CONCAT_2(x, y) CONCAT_1(x, y)

#define LOG_SS_VAR CONCAT_2(_SLXS_LOG_ss_, __LINE__)

#ifdef DEBUG_MODE
#define LOCK_LOG_FILE                                               \
    std::lock_guard<decltype(slxs::log::Log::file_guard)> CONCAT_2( \
        _SLXS_LOG_lg_, __LINE__)(slxs::log::Log::file_guard);
#else
#define LOCK_LOG_FILE
#endif

#define END_OF_MSG << std::flush

#define LOG(x, t)                                                                                    \
    {                                                                                                \
        std::stringstream LOG_SS_VAR;                                                                \
        auto CONCAT_2(_SLXS_LOG_t_, __LINE__) = std::time(nullptr);                                  \
        auto CONCAT_2(_SLXS_LOG_tm_, __LINE__) = *std::localtime(&CONCAT_2(_SLXS_LOG_t_, __LINE__)); \
        LOG_SS_VAR << std::put_time(&CONCAT_2(_SLXS_LOG_tm_, __LINE__), "%d-%m-%Y %H-%M-%S")         \
                   << " [thread:" << std::this_thread::get_id() << "]-["                             \
                   << __FILE__ << ":" << __LINE__ << "]-[" << #t << "] " << x << "\n";               \
        LOCK_LOG_FILE                                                                                \
        slxs::log::Log::file << LOG_SS_VAR.str() END_OF_MSG;                                         \
    }

#ifdef DEBUG_MODE
#define LOG_D(x) LOG(x, debug)
#else
#define LOG_D(x)
#endif

#define LOG_I(x) LOG(x, info)
#define LOG_E(x) LOG(x, error)
#define LOG_F(x)          \
    {                     \
        LOG(x, fatal)     \
        std::terminate(); \
    }

namespace slxs::log {
class Log final {
public:
#ifdef DEBUG_MODE
    static std::mutex file_guard;
#endif
    static std::ofstream file;

    static void initialize() noexcept;
};
} // namespace slxs

#endif