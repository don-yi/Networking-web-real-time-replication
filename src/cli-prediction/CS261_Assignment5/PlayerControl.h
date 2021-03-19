//---------------------------------------------------------
// file:	PlayerControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Common interface for all player-control types.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


/// <summary>
/// Common interface for all player-control types.
/// </summary>
class PlayerControl
{
public:
	virtual ~PlayerControl() = default;
	
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;

	float GetCurrentX() const { return current_x_; }
	float GetCurrentY() const { return current_y_; }

protected:
	float current_x_ = 0.0f, current_y_ = 0.0f;
};