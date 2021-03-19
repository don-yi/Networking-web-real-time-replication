//---------------------------------------------------------
// file:	Player.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Representation of a player in the game
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "Player.h"

const int kPlayerTrailLength = 15;


void Player::SetPosition(const float x, const float y)
{
	if (previous_x.size() > kPlayerTrailLength)
	{
		previous_x.pop_back();
		previous_y.pop_back();
	}
	previous_x.push_front(current_x);
	previous_y.push_front(current_y);
	current_x = x;
	current_y = y;
}


void Player::Draw() const
{
	CP_Settings_NoStroke();

	auto alpha = 255;
	auto draw_color = CP_Color_Create(color.r, color.g, color.b, alpha);
	auto draw_size = size;
	DrawPlayer(current_x, current_y, draw_size, draw_color);
	for (size_t i = 0; i < previous_x.size(); ++i)
	{
		draw_color.a -=  255 / kPlayerTrailLength;
		DrawPlayer(previous_x[i], previous_y[i], draw_size - i, draw_color);
	}
}


void Player::DrawPlayer(const float x, const float y, const float size, const CP_Color color)
{
	CP_Settings_NoStroke();
	CP_Settings_Fill(color);
	CP_Graphics_DrawCircle(x, y, size);
}