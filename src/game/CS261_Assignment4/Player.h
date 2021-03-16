//---------------------------------------------------------
// file:	Player.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Representation of a player in the game
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include <deque>


/// <summary>
/// Representation of a player in the game.
/// </summary>
struct Player
{
	float size = 25.0f;
	CP_Color color = CP_Color_Create(0, 0, 255, 255);

	void SetPosition(float x, float y);

	/// <summary>
	/// Draw the player object at its current location.
	/// </summary>
	void Draw() const;

private:
	float current_x = 0.0f;
	float current_y = 0.0f;

	std::deque<float> previous_x;
	std::deque<float> previous_y;
};