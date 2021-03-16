//---------------------------------------------------------
// file:	DoubleOrbitControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates a moving position within a double-orbital (figure-eight).
//
// remarks: This is the primary element of "simulation" in the replication testbed.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


/// <summary>
/// Calculates a moving position within a double-orbital (figure-eight).
/// </summary>
/// <remarks>This is the primary element of "simulation" in the replication testbed.</remarks>
class DoubleOrbitControl
{
public:
	DoubleOrbitControl(float left_center_x, float left_center_y, float radius, float duration_secs);

	void Update();

	inline float GetCurrentX() const { return current_x_; }
	inline float GetCurrentY() const { return current_y_; }
	
private:
	void CalculateCurrentPosition();
	
	float left_center_x_;
	float left_center_y_;
	float right_center_x_;
	float right_center_y_; 
	float radius_;
	float duration_secs_;
	bool isOrbitingLeft_;
	float current_angle_;
	float current_x_;
	float current_y_;
};

