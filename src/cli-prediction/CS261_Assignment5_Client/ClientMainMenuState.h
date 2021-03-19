//---------------------------------------------------------
// file:	ClientMainMenuState.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The main menu for the client, listing a series of scenarios.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "GameState.h"
#include "ClientConfiguration.h"


/// <summary>
/// The main menu for the client, listing a series of scenarios.
/// </summary>
class ClientMainMenuState :
    public GameState
{
public:
	ClientMainMenuState(ClientConfiguration configuration);
	~ClientMainMenuState() override;

	void Update() override;
	void Draw() override;

private:
	ClientConfiguration configuration_;
};