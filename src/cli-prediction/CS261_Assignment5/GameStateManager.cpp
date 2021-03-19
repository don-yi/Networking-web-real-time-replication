//---------------------------------------------------------
// file:	GameStateManager.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Maps GameState objects into the static C-Processing functional engine.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "GameState.h"
#include "GameStateManager.h"


GameState* base_state = nullptr;
GameState* current_state = nullptr;
GameState* next_state = nullptr;


/// <summary>
/// Per-frame update for this game state.
/// </summary>
/// <remarks>Provided to C-Processing's engine.</remarks>
void GameStateManager_Update(void)
{
	if (next_state != nullptr)
	{
		if ((current_state != nullptr) && (current_state != base_state))
		{
			delete current_state;
		}

		current_state = next_state;
		next_state = nullptr;
	}
	
	// update the current state
	if (current_state != nullptr)
	{
		current_state->Update();
	}

	// draw the current state
	CP_Settings_Background(CP_Color_Create(0, 0, 25, 255));
	if (current_state != nullptr)
	{
		current_state->Draw();
	}
}


/// <summary>
/// Add a game state to the stack.
/// </summary>
/// <param name="game_state">The latest game state.</param>
void GameStateManager::ApplyState(GameState* game_state)
{
	if ((game_state == nullptr) || (game_state == current_state))
	{
		return;
	}

	if ((next_state != nullptr) && (next_state != game_state) && (next_state != base_state))
	{
		delete next_state;
	}

	next_state = game_state;
}


/// <summary>
/// Remove the current game state from the stack.
/// </summary>
void GameStateManager::ReturnToBaseState()
{
	ApplyState(base_state);
}


/// <summary>
/// Establish the game state manager with a base state that can never be removed.
/// </summary>
/// <param name="new_base_state">The primary state for the game, likely a main menu.</param>
/// <param name="init">The init function that will be run once, as part of establishing.</param>
void GameStateManager::Establish(GameState* new_base_state, FunctionPtr init)
{	
	if ((new_base_state == nullptr) || (new_base_state == base_state))
	{
		return;
	}

	if (base_state != nullptr)
	{
		delete base_state;
	}

	base_state = new_base_state;
	ReturnToBaseState();

	CP_Engine_SetNextGameStateForced(init, GameStateManager_Update, nullptr);
}