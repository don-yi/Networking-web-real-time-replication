//---------------------------------------------------------
// file:	ConnectingMenuState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when connecting to a server
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "GameState.h"
#include "NetworkedScenarioState.h"
#include "ClientConfiguration.h"


/// <summary>
/// The menu shown when connecting to a server
/// </summary>
class ConnectingMenuState :
    public GameState
{
public:
    ConnectingMenuState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ClientConfiguration configuration);
    ~ConnectingMenuState() override;

    // Inherited via GameState
    virtual void Update() override;
    virtual void Draw() override;

private:
    void SendConnectionRequest();
    bool HandleSocketError(const char* error_text);

    NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator_;
    std::string game_type_;
    ClientConfiguration configuration_;

    SOCKET connecting_socket_;
    float connecting_timer_secs_;
    char* network_buffer_;

    std::string operation_description_;
};