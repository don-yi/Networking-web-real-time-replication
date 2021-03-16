//---------------------------------------------------------
// file:	NetworkedScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Shared data and functionality for all scenarios that involve networking.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "ScenarioState.h"


/// <summary>
/// Shared data and functionality for all scenarios that involve networking.
/// </summary>
class NetworkedScenarioState :
    public ScenarioState
{
public:
    NetworkedScenarioState(const SOCKET socket, const bool is_host);
    ~NetworkedScenarioState() override;

    // Inherited via GameState
    virtual void Update() override;

    typedef NetworkedScenarioState* (*NetworkedScenarioStateCreator)(const SOCKET, const bool);

protected:
    SOCKET socket_;
    bool is_host_;
};