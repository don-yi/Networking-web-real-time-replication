//---------------------------------------------------------
// file:	OptimisticClientScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario for the client, in which the remote is the authority.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "NetworkedScenarioState.h"
#include "Player.h"
#include "SnapshotControl.h"
#include "DirectControl.h"
#include "Packet.h"
#include "OptimisticAttack.h"


/// <summary>
/// The optimistic scenario for the client, in which the remote is the authority.
/// </summary>
class OptimisticClientScenarioState final :
    public NetworkedScenarioState
{
public:
    OptimisticClientScenarioState(const SOCKET socket);
    ~OptimisticClientScenarioState() override;

    OptimisticClientScenarioState(const OptimisticClientScenarioState&) = delete;
    OptimisticClientScenarioState(OptimisticClientScenarioState&&) = delete;
    OptimisticClientScenarioState& operator=(const OptimisticClientScenarioState&) = delete;
    OptimisticClientScenarioState& operator=(OptimisticClientScenarioState&&) = delete;

    void Update() override;
    void Draw() override;

    std::string GetDescription() const override;
    std::string GetInstructions() const override;

private:
    bool HandleSocketError(const char* error_text);

    SnapshotControl snapshot_remote_control_;
    DirectControl direct_local_control_;
    bool is_drawing_controls_;

    Player local_player_;
    Player remote_player_;

    std::unique_ptr<OptimisticAttack> local_attack_;
    bool is_attack_queued_;

    u_long local_frame_;
    u_long remote_frame_;
    float send_timer_secs_;
    float time_since_last_recv_;

    //TODO: define local movement record, containing:
    // 1) the local frame 
    // 2) the delta between the last frame's position and this frame's position, as x and y components

    //TODO: define a history of local movement records, as a deque of the local movement records

    Packet packet_;
};