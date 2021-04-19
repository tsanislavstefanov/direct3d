#include "stdafx.h"
#include "Stopwatch.h"

float Stopwatch::GetSeconds() const
{
    return GetMilliseconds() * 0.001f;
}

float Stopwatch::GetMilliseconds() const
{
    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - m_Start);
    return duration.count() * 0.000001f;
}

void Stopwatch::Now()
{
    m_Start = std::chrono::high_resolution_clock::now();
}