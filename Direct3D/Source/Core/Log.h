#pragma once

#include <spdlog/spdlog.h>

struct Log
{
    Log() = delete;
    Log(const Log&) = delete;

    Log& operator=(const Log&) = delete;

    static void Init();

    static const std::shared_ptr<spdlog::logger> Main;
};

#ifdef _DEBUG
    #define LOG_INFO(...) Log::Main->info(__VA_ARGS__)
    #define LOG_WARNING(...) Log::Main->warn(__VA_ARGS__)
    #define LOG_ERROR(...) Log::Main->error(__VA_ARGS__)
#else
    #define LOG_INFO(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...)
#endif