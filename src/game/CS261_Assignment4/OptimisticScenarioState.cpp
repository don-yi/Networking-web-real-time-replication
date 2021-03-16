//---------------------------------------------------------
// file:	OptimisticScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario, in which the both the host and client run some kind of simulation, but the host is the authority
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "OptimisticScenarioState.h"


OptimisticScenarioState::OptimisticScenarioState(const SOCKET socket, const bool is_host)
    : NetworkedScenarioState(socket, is_host)
{
}

OptimisticScenarioState::~OptimisticScenarioState() = default;


void OptimisticScenarioState::Update()
{
	NetworkedScenarioState::Update();
}


void OptimisticScenarioState::Draw()
{
    ScenarioState::Draw();
}


std::string OptimisticScenarioState::GetDescription() const
{
	std::string description("Optimistic Scenario, ");
	description += is_host_ ? "Host" : "Non-Host";
	return description;
}


std::string OptimisticScenarioState::GetInstructions() const
{
	return std::string();
}