//---------------------------------------------------------
// file:	ClientMainMenuState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The main menu for the client, listing a series of scenarios.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "framework.h"
#include "GameStateManager.h"
#include "ClientMainMenuState.h"
#include "UserLoginState.h"
#include "SinglePlayerScenarioState.h"
#include "LockstepScenarioState.h"
#include "DumbClientScenarioState.h"
#include "OptimisticScenarioState.h"


/// <summary>
/// The size of the description text.
/// </summary>
const float kMenuOptionTextSize = 30.0f;

/// <summary>
/// The color of the description text.
/// </summary>
const CP_Color kMenuOptionTextColor = CP_Color_Create(255, 255, 255, 255);


ClientMainMenuState::ClientMainMenuState(ClientConfiguration configuration)
	: configuration_(configuration)
{ }


ClientMainMenuState::~ClientMainMenuState() = default;


void ClientMainMenuState::Update()
{
	// if the user presses ESC from the main menu, the process will exit.
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		CP_Engine_Terminate();
		return;
	}

	// transition between game modes
	// -- note that redundant selections will still reset the game state
	if (CP_Input_KeyTriggered(KEY_1) || CP_Input_KeyTriggered(KEY_KP_1))
	{
		auto* game_state = new SinglePlayerScenarioState();
		GameStateManager::ApplyState(game_state);
	}
	else if (CP_Input_KeyTriggered(KEY_2) || CP_Input_KeyTriggered(KEY_KP_2))
	{
		auto* game_state = new UserLoginState(
			[](const SOCKET socket, const bool is_host) -> NetworkedScenarioState*
			{ 
				return new LockstepScenarioState(socket, is_host); 
			}, "Lockstep", configuration_);
		GameStateManager::ApplyState(game_state);
	}
	else if (CP_Input_KeyTriggered(KEY_3) || CP_Input_KeyTriggered(KEY_KP_3))
	{
		auto* game_state = new UserLoginState(
			[](const SOCKET socket, const bool is_host) -> NetworkedScenarioState*
			{
				return new DumbClientScenarioState(socket, is_host);
			}, "DumbClient", configuration_);
		GameStateManager::ApplyState(game_state);
	}
	else if (CP_Input_KeyTriggered(KEY_4) || CP_Input_KeyTriggered(KEY_KP_4))
	{
		auto* game_state = new UserLoginState(
			[](const SOCKET socket, const bool is_host) -> NetworkedScenarioState*
			{
				return new OptimisticScenarioState(socket, is_host);
			}, "Optimistic", configuration_);
		GameStateManager::ApplyState(game_state);
	}
}


void ClientMainMenuState::Draw()
{
	// draw the menu options
	CP_Settings_TextSize(kMenuOptionTextSize);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(kMenuOptionTextColor);
	CP_Font_DrawText("CLIENT MENU", 10.0f, 10.0f);
	CP_Font_DrawText("Press 1 for Single Player", 10.0f, 40.0f);
	CP_Font_DrawText("Press 2 for Lockstep (2 player)", 10.0f, 70.0f);
	CP_Font_DrawText("Press 3 for Dumb Client (2 player)", 10.0f, 100.0f);
	CP_Font_DrawText("Press 4 for Optimistic (2 player)", 10.0f, 130.0f);
	CP_Settings_Stroke(kMenuOptionTextColor);
	CP_Graphics_DrawLine(10.0f, 38.0f, 167.0f, 38.0f);
}