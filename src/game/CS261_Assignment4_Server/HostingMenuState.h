//---------------------------------------------------------
// file:	HostingMenuState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when hosting a server, and waiting for a user
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "framework.h"
#include "GameState.h"
#include "NetworkedScenarioState.h"
#include "ServerConfiguration.h"


/// <summary>
/// The menu shown when hosting a server, and waiting for a user
/// </summary>
class HostingMenuState :
    public GameState
{
public:
    HostingMenuState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ServerConfiguration configuration);
    ~HostingMenuState() override;

    // Inherited via GameState
    virtual void Update() override;
    virtual void Draw() override;

private:
    bool HandleSocketError(const char* error_text);

    void SendConnectionSuccess(SOCKADDR_IN other_address);
    void SendConnectionFailure(SOCKADDR_IN other_address, const char* message);

    NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator_;
    std::string game_type_;
    ServerConfiguration configuration_;

    SOCKET hosting_socket_;
    char* network_buffer_;

    std::string operation_description_;
};