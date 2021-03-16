//---------------------------------------------------------
// file:	SinglePlayerScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The single-player scenario, provided for simple testing of player control and rendering
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "SinglePlayerScenarioState.h"
#include "GameStateManager.h"


SinglePlayerScenarioState::SinglePlayerScenarioState()
	: double_orbit_control_(250.0f, 250.0f, 100.0f, 1.0f)
{
	player_.color = CP_Color_Create(255, 0, 0, 255);
}


SinglePlayerScenarioState::~SinglePlayerScenarioState() = default;


void SinglePlayerScenarioState::Update()
{
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		GameStateManager::ReturnToBaseState();
		return;
	}

	if (!CP_Input_KeyDown(KEY_SPACE))
	{
        double_orbit_control_.Update();
	}

    player_.SetPosition(double_orbit_control_.GetCurrentX(), double_orbit_control_.GetCurrentY());
}


void SinglePlayerScenarioState::Draw()
{
    ScenarioState::Draw();
    player_.Draw();

    CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
    CP_Settings_NoStroke();
}


std::string SinglePlayerScenarioState::GetDescription() const
{
    return "Single-Player (Local) Scenario";
}


std::string SinglePlayerScenarioState::GetInstructions() const
{
	return "Hold SPACE to halt the local (red) player";
}