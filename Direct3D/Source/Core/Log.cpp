#include "stdafx.h"

#include <spdlog/sinks/stdout_color_sinks.h>

#include "Log.h"

const std::shared_ptr<spdlog::logger> Log::Main = spdlog::stdout_color_mt("MAIN");

void Log::Init()
{
    // Set-up level and pattern.
    Main->set_level(spdlog::level::trace);
    Main->set_pattern("%^[%T.%e]: %v%$");

    // Change sink of info log level.
    std::vector<spdlog::sink_ptr>& sinks = Main->sinks();
    auto sink = static_cast<spdlog::sinks::stdout_color_sink_mt*>(sinks[0].get());
    sink->set_color(spdlog::level::info, 3);
}