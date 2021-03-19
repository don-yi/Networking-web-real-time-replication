//---------------------------------------------------------
// file:	GameStateManager.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Maps GameState objects into the static C-Processing functional engine.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once

class GameState;


namespace GameStateManager
{
	void ApplyState(GameState* game_state);
	void ReturnToBaseState();

	void Establish(GameState* base_game_state, FunctionPtr init);
};

