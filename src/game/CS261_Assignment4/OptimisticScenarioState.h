//---------------------------------------------------------
// file:	OptimisticScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario, in which the both the host and client run some kind of simulation, but the host is the authority
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "NetworkedScenarioState.h"


/// <summary>
/// The optimistic scenario, in which the both the host and client run some kind of simulation, but the host is the authority
/// </summary>
class OptimisticScenarioState final :
    public NetworkedScenarioState
{
public:
    OptimisticScenarioState(const SOCKET socket, const bool is_host);
    ~OptimisticScenarioState() override;

    void Update() override;
    void Draw() override;

    std::string GetDescription() const override;
    std::string GetInstructions() const override;
};