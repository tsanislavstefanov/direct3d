#include "stdafx.h"

#ifdef _WIN32
    #undef _WIN32
#endif

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.h"

std::shared_ptr<spdlog::logger> Log::s_Main = nullptr;

spdlog::logger& Log::GetMain()
{
    return *s_Main;
}

void Log::Init()
{
    // Create and set-up level and pattern.
    s_Main = spdlog::stdout_color_mt("MAIN");
    s_Main->set_level(spdlog::level::trace);
    s_Main->set_pattern("%^[%T.%e]: %v%$");

    // Change sink of info log level.
    std::vector<spdlog::sink_ptr>& sinks = s_Main->sinks();
    auto sink = static_cast<spdlog::sinks::stdout_color_sink_mt*>(sinks[0].get());
    sink->set_color(spdlog::level::info, sink->cyan);
}