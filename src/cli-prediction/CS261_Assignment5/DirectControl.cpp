//---------------------------------------------------------
// file:	DirectControl.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Updates a position based on direct player controls (pressing the four cardinal directions).
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "DirectControl.h"

const float kPlayerVelocity = 100.0f;


void DirectControl::Update(const float dt)
{
	const auto delta = kPlayerVelocity * dt;
	
	if (up_ && !down_)
	{
		current_y_ -= delta;
	}
	if (down_ && !up_)
	{
		current_y_ += delta;
	}
	if (left_ && !right_)
	{
		current_x_ -= delta;
	}
	if (right_ && !left_)
	{
		current_x_ += delta;
	}
}


void DirectControl::Draw()
{
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_Fill(CP_Color_Create(0, 255, 0, 255));
	CP_Graphics_DrawCircle(current_x_, current_y_, 15);
	CP_Graphics_DrawCircle(last_remote_x_, last_remote_y_, 15);
	CP_Graphics_DrawLine(current_x_, current_y_, last_remote_x_, last_remote_y_);
}