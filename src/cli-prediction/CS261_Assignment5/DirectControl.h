//---------------------------------------------------------
// file:	DirectControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Updates a position based on direct player controls (pressing the four cardinal directions).
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include "PlayerControl.h"


/// <summary>
/// Updates a position based on direct player controls (pressing the four cardinal directions).
/// </summary>
class DirectControl final
	: public PlayerControl
{
public:
	void SetPosition(const float x, const float y) { current_x_ = x; current_y_ = y; }
	void SetLastRemotePosition(const float x, const float y) { last_remote_x_ = x; last_remote_y_ = y; }
	void SetControls(const bool up, const bool down, const bool left, const bool right) { up_ = up; down_ = down; left_ = left; right_ = right; }
	
	void Update(float dt) override;
	void Draw() override;

private:
	bool up_ = false;
	bool down_ = false;
	bool left_ = false;
	bool right_ = false;

	float last_remote_x_ = 0.0f, last_remote_y_ = 0.0f;
};