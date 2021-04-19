#include "stdafx.h"
#include "Time.h"

float Time::Scale = 1.0f;
float Time::s_Seconds = 0.0f;
float Time::s_FixedSeconds = 0.0f;
uint32_t Time::s_FrameCount = 0;

float Time::GetSeconds()
{
    return s_Seconds;
}

float Time::GetFixedSeconds()
{
    return s_FixedSeconds;
}

uint32_t Time::GetFrameCount()
{
    return s_FrameCount;
}