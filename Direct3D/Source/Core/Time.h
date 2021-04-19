#pragma once

class Time
{
public:
    static constexpr float FixedTime = 1.0f / 60.0f;

    Time() = delete;
    Time(const Time&) = delete;

    Time& operator=(const Time&) = delete;

    static float GetSeconds();
    static float GetFixedSeconds();
    static uint32_t GetFrameCount();

    static float Scale;
private:
    static float s_Seconds;
    static float s_FixedSeconds;
    static uint32_t s_FrameCount;

    friend class Application;
};