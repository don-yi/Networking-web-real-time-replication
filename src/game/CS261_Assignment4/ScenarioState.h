//---------------------------------------------------------
// file:	Scenario.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Shared data and functionality for all scenarios
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include "GameState.h"


/// <summary>
/// Shared data and functionality for all scenarios
/// </summary>
class ScenarioState :
    public GameState
{
public:
    ScenarioState();
    ~ScenarioState() override;

    virtual void Draw() override;

    virtual std::string GetDescription() const = 0;
    virtual std::string GetInstructions() const = 0;
};