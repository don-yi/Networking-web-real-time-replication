//---------------------------------------------------------
// file:	Clock.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	High-precision calculation of time deltas. 
// 
// NOTE: Users of this class must establish their own initial value using QueryPerformanceCounter.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


/// <summary>
/// High-precision calculation of time deltas. 
/// </summary>
/// <remarks>Users of this class must establish their own initial value using QueryPerformanceCounter.</remarks>
namespace Clock
{
	float GetDt(LARGE_INTEGER& perf_counter);
};