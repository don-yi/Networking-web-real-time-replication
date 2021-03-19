//---------------------------------------------------------
// file:	GameState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Base class for all GameStates, filling expectations for C-Processing.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


class GameState
{
public:
	GameState();
	virtual ~GameState();
	
	virtual void Update() = 0;
	virtual void Draw() = 0;
};