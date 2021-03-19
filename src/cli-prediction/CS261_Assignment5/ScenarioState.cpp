//---------------------------------------------------------
// file:	Scenario.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Shared data and functionality for all scenarios
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "ScenarioState.h"

const float kDescriptionTextSize = 25.0f; // The size of the description text.
const CP_Color kDescriptionTextColor = CP_Color_Create(255, 255, 255, 255); // The color of the description text.


ScenarioState::ScenarioState() = default;
ScenarioState::~ScenarioState() = default;


void ScenarioState::Draw()
{
	// draw the description
	CP_Settings_TextSize(kDescriptionTextSize);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(kDescriptionTextColor);
	CP_Font_DrawText(GetDescription().c_str(), 0.0f, 0.0f);
	CP_Font_DrawText(GetInstructions().c_str(), 0.0f, 740.0f);
}