#pragma once

class Stopwatch
{
public:
    float GetSeconds() const;
    float GetMilliseconds() const;

    void Now();
private:
    std::chrono::time_point<std::chrono::steady_clock> m_Start = {};
};