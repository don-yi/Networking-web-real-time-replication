//---------------------------------------------------------
// file:	DoubleOrbitControl.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates a moving position within a double-orbital (figure-eight).
//
// remarks: This motion represents "unpredictable" player control, in our scenarios.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "DoubleOrbitControl.h"
#include "LabMath.h"


DoubleOrbitControl::DoubleOrbitControl(const float left_center_x, const float left_center_y, const float radius,
                                       const float duration_secs)
	: left_center_x_(left_center_x),
	  left_center_y_(left_center_y),
	  right_center_x_(left_center_x + (2.0f * radius)),
	  right_center_y_(left_center_y),
	  radius_(radius),
	  duration_secs_(duration_secs),
	  current_velocity_x_(0.0f),
  	  current_velocity_y_(0.0f)
{
	current_x_ = CalculateX(current_state_);
	current_y_ = CalculateY(current_state_);
}


void DoubleOrbitControl::Update(float dt)
{
	// update the current angle deterministically, even if that time is inaccurate
	current_state_.angle += dt * (LabMath::kTwoPi / duration_secs_);

	// swap the center each time we orbit
	if (current_state_.angle > LabMath::kTwoPi)
	{
		current_state_.is_orbiting_left = !current_state_.is_orbiting_left;
		current_state_.angle = fmod(current_state_.angle, LabMath::kTwoPi);
	}

	float last_x = current_x_, last_y = current_y_;
	current_x_ = CalculateX(current_state_);
	current_y_ = CalculateY(current_state_);
	current_velocity_x_ = (dt > 0.0f) ? (current_x_ - last_x) / dt : 0.0f;
	current_velocity_y_ = (dt > 0.0f) ? (current_y_ - last_y) / dt : 0.0f;
}


float DoubleOrbitControl::CalculateX(const State& state) const
{
	if (state.is_orbiting_left)
	{
		return left_center_x_ + (cos(state.angle) * radius_);
	}
	else
	{
		return right_center_x_ - (cos(state.angle) * radius_);
	}
}


float DoubleOrbitControl::CalculateY(const State& state) const
{
	if (state.is_orbiting_left)
	{
		return left_center_y_ + (sin(state.angle) * radius_);
	}
	else
	{
		return right_center_y_ + (sin(state.angle) * radius_);
	}
}

DoubleOrbitControl::State DoubleOrbitControl::State::CalculateIntermediateState(const DoubleOrbitControl::State& base_state, const DoubleOrbitControl::State& target_state, const float t)
{
	if (t <= 0.0f)
	{
		return base_state;
	}
	else if (t >= 1.0f)
	{
		return target_state;
	}
	DoubleOrbitControl::State result{};

	result.is_orbiting_left = base_state.is_orbiting_left;
	auto angle = base_state.angle + (target_state.angle - base_state.angle) * t;
	if (angle > LabMath::kTwoPi)
	{
		angle = fmod(angle, LabMath::kTwoPi);
		result.is_orbiting_left = target_state.is_orbiting_left;
	}
	result.angle = angle;

	return result;
}