//---------------------------------------------------------
// file:	DeadReckoningControl.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates the position as *predicted* from the last known position and velocity.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "DeadReckoningControl.h"
#include "LabMath.h"


void DeadReckoningControl::SetLastKnown(const float position_x, const float position_y, 
	const float velocity_x, const float velocity_y, const float time_since_last_update_secs, const u_long remote_frame)
{
	remote_frame_ = remote_frame;
	
	last_known_position_x_ = position_x;
	last_known_position_y_ = position_y;
	last_known_velocity_x_ = velocity_x;
	last_known_velocity_y_ = velocity_y;

	if (is_initialized_ == false)
	{
		current_x_ = position_x;
		current_y_ = position_y;
		current_velocity_x_ = velocity_x;
		current_velocity_y_ = velocity_y;
		current_acceleration_x_ = 0.0f;
		current_acceleration_y_ = 0.0f;
		is_initialized_ = true;
		return;
	}

	current_acceleration_x_ = (velocity_x - current_velocity_x_) / time_since_last_update_secs;
	current_acceleration_y_ = (velocity_y - current_velocity_y_) / time_since_last_update_secs;

	//if (LabMath::IsWithinDistance(position_x, position_y, current_x_, current_y_, 100.0f) == false)
	//{
	//	current_x_ = position_x;
	//	current_y_ = position_y;
	//}
}


void DeadReckoningControl::Update(const float dt)
{
	if (is_initialized_ == false)
	{
		return;
	}

	current_velocity_x_ += current_acceleration_x_ * dt;
	current_velocity_y_ += current_acceleration_y_ * dt;

	current_x_ += current_velocity_x_ * dt;
	current_y_ += current_velocity_y_ * dt;
}


void DeadReckoningControl::Draw()
{
	// draw the last-known position and velocity in purple
	// -- note, the velocity values are basically the distance for 1 second of travel... which is too long.  scale it down...
	const auto velocity_scale = 0.25f;
	CP_Settings_Stroke(CP_Color_Create(255, 0, 255, 255));
	CP_Settings_Fill(CP_Color_Create(255, 0, 255, 255));
	CP_Graphics_DrawCircle(last_known_position_x_, last_known_position_y_, 30.0f);
	CP_Graphics_DrawLine(last_known_position_x_, last_known_position_y_,
		last_known_position_x_ + last_known_velocity_x_ * velocity_scale,
		last_known_position_y_ + last_known_velocity_y_ * velocity_scale);

	// draw the current velocity and acceleration
	CP_Settings_Stroke(CP_Color_Create(0, 255, 0, 255));
	CP_Graphics_DrawLine(current_x_, current_y_,
		current_x_ + current_velocity_x_ * velocity_scale,
		current_y_ + current_velocity_y_ * velocity_scale);
}


SyncRatio DeadReckoningControl::GetSyncRatio() const
{
	const struct SyncRatio sync_ratio { remote_frame_, remote_frame_, 0.0f };
	return sync_ratio;
}