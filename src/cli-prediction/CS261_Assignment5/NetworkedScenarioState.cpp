//---------------------------------------------------------
// file:	NetworkedScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Shared data and functionality for all scenarios that involve networking.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "NetworkedScenarioState.h"
#include "GameStateManager.h"
#include "Clock.h"


NetworkedScenarioState::NetworkedScenarioState(const SOCKET socket, const bool is_host)
	: socket_(socket), is_host_(is_host)
{
	QueryPerformanceCounter(&perf_last);
}


NetworkedScenarioState::~NetworkedScenarioState() = default;


void NetworkedScenarioState::Update()
{
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		if (socket_ != INVALID_SOCKET)
		{
			closesocket(socket_);
			socket_ = INVALID_SOCKET;
		}
		GameStateManager::ReturnToBaseState();
		return;
	}

	dt = Clock::GetDt(perf_last);
}