//---------------------------------------------------------
// file:	LockstepScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The lockstep scenario, in which the both the host and client simulate and must stay in perfect sync
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "LockstepScenarioState.h"
#include "PacketSerializer.h"

const int kNetworkBufferSize = 1024;


LockstepScenarioState::LockstepScenarioState(const SOCKET socket, const bool is_host)
	: NetworkedScenarioState(socket, is_host),
	  host_control_(200.0f, 250.0f, 100.0f, 1.0f),
	  non_host_control_(200.0f, 150.0f, 100.0f, 2.0f),
	  isRemotePaused_(false),
	  local_frame_(0),
	  remote_frame_(0),
	  packet_(kNetworkBufferSize)
{
	local_player_.color = CP_Color_Create(255, 0, 0, 255);
	remote_player_.color = CP_Color_Create(0, 0, 255, 255);
}


LockstepScenarioState::~LockstepScenarioState() = default;


void LockstepScenarioState::Update()
{
	NetworkedScenarioState::Update();

	auto* local_control = is_host_ ? &host_control_ : &non_host_control_;
	auto* remote_control = is_host_ ? &non_host_control_ : &host_control_;

	// if we are behind the remote, update until we catch up
	if (local_frame_ <= remote_frame_)
	{
		const bool is_local_paused = CP_Input_KeyDown(KEY_SPACE);
		// both the host and client update the simulation
		if (!is_local_paused)
		{
			local_control->Update();
		}
		if (!isRemotePaused_)
		{
			remote_control->Update();
		}

		packet_.Reset();
		PacketSerializer::WriteValue<u_long>(packet_, ++local_frame_);
		PacketSerializer::WriteValue<bool>(packet_, is_local_paused);

		send(socket_, packet_.GetRoot(), packet_.GetUsedSpace(), 0);
	}

	// if the remote is behind us, look for updates on the network
	if (remote_frame_ <= local_frame_)
	{
		packet_.Reset();
		const auto res = recv(socket_, packet_.GetRoot(), packet_.GetRemainingSpace(), 0);
		if (res > 0)
		{
			u_long received_frame;
			PacketSerializer::ReadValue<u_long>(packet_, received_frame);
			if (received_frame > remote_frame_)
			{
				remote_frame_ = received_frame;
				PacketSerializer::ReadValue<bool>(packet_, isRemotePaused_);
			}
		}
	}

	// apply whatever information we have
	local_player_.SetPosition(local_control->GetCurrentX(), local_control->GetCurrentY());
	remote_player_.SetPosition(remote_control->GetCurrentX(), remote_control->GetCurrentY());
}


void LockstepScenarioState::Draw()
{
    ScenarioState::Draw();

	local_player_.Draw();
	remote_player_.Draw();
}


std::string LockstepScenarioState::GetDescription() const
{
	std::string description("Lockstep Scenario, ");
	description += is_host_ ? "Host, " : "Non-Host, ";
	description += "Local: ";
	description += std::to_string(local_frame_);
	description += ", Remote: ";
	description += std::to_string(remote_frame_);
	return description;
}


std::string LockstepScenarioState::GetInstructions() const
{
	return "Hold SPACE to halt the local (red) player";
}


bool LockstepScenarioState::HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << "Lockstep WinSock Error: " << error_text << wsa_error << std::endl;

	// close the socket and clear it
	// -- this should trigger a GameStateManager reset in the next Update
	closesocket(socket_);
	socket_ = INVALID_SOCKET;

	return true;
}