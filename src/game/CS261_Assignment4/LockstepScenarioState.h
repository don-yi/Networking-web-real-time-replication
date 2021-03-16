//---------------------------------------------------------
// file:	LockstepScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The lockstep scenario, in which the both the host and client simulate and must stay in perfect sync
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "DoubleOrbitControl.h"
#include "Player.h"
#include "NetworkedScenarioState.h"
#include "Packet.h"


/// <summary>
/// The lockstep scenario, in which the both the host and client simulate and must stay in perfect sync
/// </summary>
class LockstepScenarioState final:
    public NetworkedScenarioState
{
public:
    LockstepScenarioState(const SOCKET socket, const bool is_host);
    ~LockstepScenarioState() override;

    LockstepScenarioState(const LockstepScenarioState&) = delete;
    LockstepScenarioState(LockstepScenarioState&&) = delete;
    LockstepScenarioState& operator=(const LockstepScenarioState&) = delete;
    LockstepScenarioState& operator=(LockstepScenarioState&&) = delete;

    void Update() override;
    void Draw() override;

    std::string GetDescription() const override;
    std::string GetInstructions() const override;
	
private:
    bool HandleSocketError(const char* error_text);

    DoubleOrbitControl host_control_;
    DoubleOrbitControl non_host_control_;

    Player local_player_;
    Player remote_player_;

    bool isRemotePaused_;

    u_long local_frame_;
    u_long remote_frame_;

    Packet packet_;
};