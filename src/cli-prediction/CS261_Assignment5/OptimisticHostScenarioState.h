//---------------------------------------------------------
// file:	OptimisticHostScenarioState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario for the host, in which the local system is the authority.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "NetworkedScenarioState.h"
#include "Player.h"
#include "Packet.h"
#include "DoubleOrbitControl.h"
#include "SnapshotControl.h"
#include "OptimisticAttack.h"
#include "DirectControl.h"


/// <summary>
/// The optimistic scenario for the host, in which the local system is the authority.
/// </summary>
class OptimisticHostScenarioState final :
    public NetworkedScenarioState
{
public:
    OptimisticHostScenarioState(const SOCKET socket);
    ~OptimisticHostScenarioState() override;

    OptimisticHostScenarioState(const OptimisticHostScenarioState&) = delete;
    OptimisticHostScenarioState(OptimisticHostScenarioState&&) = delete;
    OptimisticHostScenarioState& operator=(const OptimisticHostScenarioState&) = delete;
    OptimisticHostScenarioState& operator=(OptimisticHostScenarioState&&) = delete;

    void Update() override;
    void Draw() override;

    std::string GetDescription() const override;
    std::string GetInstructions() const override;

private:
    bool HandleSocketError(const char* error_text);

    DoubleOrbitControl local_control_;
    DirectControl remote_control_;

    Player local_player_;
    Player remote_player_;

    std::unique_ptr<OptimisticAttack> client_attack_;
    bool is_client_attack_confirm_queued_;

    u_long local_frame_;
    u_long remote_frame_;
    float send_timer_secs_;
    float target_time_between_send_;

    Packet packet_;

    struct ControlStateRecord
    {
        u_long frame;
        DoubleOrbitControl::State actual_control_state;
        SnapshotControl::State snapshot_state;
    };
    std::deque<ControlStateRecord> local_state_history_;
};