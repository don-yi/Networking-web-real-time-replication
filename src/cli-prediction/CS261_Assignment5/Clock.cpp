//---------------------------------------------------------
// file:	Clock.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	High-precision calculation of time deltas. 
// 
// NOTE: Users of this class must establish their own initial value using QueryPerformanceCounter.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "Clock.h"

float perf_freq = 0.0f;


float Clock::GetDt(LARGE_INTEGER& perf_counter)
{
    // static initialization - the frequency doesn't change during execution
    if (perf_freq == 0.0f)
    {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        perf_freq = (float)freq.QuadPart;
    }

    LARGE_INTEGER perf_current;
    QueryPerformanceCounter(&perf_current);

    float dt = (perf_current.QuadPart - perf_counter.QuadPart) / perf_freq;
    perf_counter = perf_current;

    return dt;
}