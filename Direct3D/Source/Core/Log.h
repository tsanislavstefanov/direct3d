#pragma once

#include <spdlog/spdlog.h>

class Log
{
public:
    Log() = delete;
    Log(const Log&) = delete;

    Log& operator=(const Log&) = delete;

    static spdlog::logger& GetMain();

    static void Init();
private:
    static std::shared_ptr<spdlog::logger> s_Main;
};

#ifdef _DEBUG
    #define LOG_INFO(...) Log::GetMain().info(__VA_ARGS__)
    #define LOG_WARNING(...) Log::GetMain().warn(__VA_ARGS__)
    #define LOG_ERROR(...) Log::GetMain().error(__VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...)
#endif