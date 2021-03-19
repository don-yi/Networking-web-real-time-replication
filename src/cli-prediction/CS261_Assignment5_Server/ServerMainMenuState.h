//---------------------------------------------------------
// file:	ServerMainMenuState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The main menu for the server, listing a series of scenarios.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "GameState.h"
#include "ServerConfiguration.h"


/// <summary>
/// The main menu for the server, listing a series of scenarios.
/// </summary>
class ServerMainMenuState :
    public GameState
{
public:
	ServerMainMenuState(ServerConfiguration configuration);
	~ServerMainMenuState() override;

	void Update() override;
	void Draw() override;

private:
	ServerConfiguration configuration_;
};