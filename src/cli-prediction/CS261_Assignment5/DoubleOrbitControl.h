//---------------------------------------------------------
// file:	DoubleOrbitControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates a moving position within a double-orbital (figure-eight).
//
// remarks: This motion represents "unpredictable" player control, in our scenarios.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "PlayerControl.h"


/// <summary>
/// Calculates a moving position within a double-orbital (figure-eight).
/// </summary>
/// <remarks>This motion represents "unpredictable" player control, in our scenarios.</remarks>
class DoubleOrbitControl final
	: public PlayerControl
{
public:
	DoubleOrbitControl(float left_center_x, float left_center_y, float radius, float duration_secs);

	virtual void Update(float dt) override;
	virtual void Draw() override { }

	inline float GetCurrentVelocityX() const { return current_velocity_x_; }
	inline float GetCurrentVelocityY() const { return current_velocity_y_; }

	struct State
	{
		float angle = 0.0f;
		bool is_orbiting_left = true;

		static State CalculateIntermediateState(const DoubleOrbitControl::State& base_state, const DoubleOrbitControl::State& target_state, float t);
	};
	inline State GetState() const { return current_state_; }

	float CalculateX(const State& state) const;
	float CalculateY(const State& state) const;

private:
	
	float left_center_x_, left_center_y_;
	float right_center_x_, right_center_y_; 
	float radius_;
	float duration_secs_;

	float current_velocity_x_ = 0.0f, current_velocity_y_ = 0.0f;
	State current_state_;
};