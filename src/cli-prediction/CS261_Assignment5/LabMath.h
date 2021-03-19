//---------------------------------------------------------
// file:	LabMath.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Common mathematical helper functions and values
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


namespace LabMath
{
	const float kTwoPi = static_cast<float>(M_PI) * 2.0f;

	bool IsWithinDistance(const float a_x, const float a_y, const float b_x, const float b_y, const float distance);
};