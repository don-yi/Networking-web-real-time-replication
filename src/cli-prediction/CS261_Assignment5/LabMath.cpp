//---------------------------------------------------------
// file:	LabMath.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Common mathematical helper functions and values
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "LabMath.h"


bool LabMath::IsWithinDistance(const float a_x, const float a_y, const float b_x, const float b_y, const float distance)
{
	const auto actual_distance = CP_Math_Distance(a_x, a_y, b_x, b_y);
	return actual_distance <= distance;
}