//---------------------------------------------------------
// file:	SinglePlayerScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The single-player scenario, provided for simple testing of player control and rendering
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "DoubleOrbitControl.h"
#include "Player.h"
#include "ScenarioState.h"


/// <summary>
/// The single-player scenario, provided for simple testing of player control and rendering
/// </summary>
class SinglePlayerScenarioState final :
    public ScenarioState
{
public:
    SinglePlayerScenarioState();
    ~SinglePlayerScenarioState() override;

    void Update() override;
    void Draw() override;
    
    std::string GetDescription() const override;
    std::string GetInstructions() const override;
	
private:
    Player player_;
    DoubleOrbitControl double_orbit_control_;
};