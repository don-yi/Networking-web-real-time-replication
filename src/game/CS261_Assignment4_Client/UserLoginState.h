//---------------------------------------------------------
// file:	UserLoginState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when the user is connecting to the user service, retrieving the necessary configuration
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "GameState.h"
#include "NetworkedScenarioState.h"
#include "ClientConfiguration.h"


/// <summary>
/// The menu shown when the user is connecting to the user service, retrieving the necessary configuration
/// </summary>
class UserLoginState :
    public GameState
{
public:
    UserLoginState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ClientConfiguration configuration);
    ~UserLoginState() override;

    // Inherited via GameState
    virtual void Update() override;
    virtual void Draw() override;

private:
    void InitiateUserRequest();

    NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator_;
    std::string game_type_;
    ClientConfiguration configuration_;

    web::http::client::http_client web_client_;
    pplx::task<web::json::value> active_task_;

    std::string operation_description_;
};