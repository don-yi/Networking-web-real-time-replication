//---------------------------------------------------------
// file:	DumbClientScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The dumb client scenario, in which the host simulates and the player wholly relies on replicated data
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "DumbClientScenarioState.h"
#include "PacketSerializer.h"

const int kNetworkBufferSize = 1024;


DumbClientScenarioState::DumbClientScenarioState(const SOCKET socket, const bool is_host)
	: NetworkedScenarioState(socket, is_host),
	host_control_(200.0f, 250.0f, 100.0f, 1.0f),
	non_host_control_(200.0f, 150.0f, 100.0f, 2.0f),
	is_remote_paused_(false),
	local_frame_(0),
	remote_frame_(0),
	is_frame_waiting_(true),
	packet_(kNetworkBufferSize)
{
	local_player_.color = CP_Color_Create(255, 0, 0, 255);
	remote_player_.color = CP_Color_Create(0, 0, 255, 255);
}


DumbClientScenarioState::~DumbClientScenarioState() = default;


void DumbClientScenarioState::Update()
{
	NetworkedScenarioState::Update();

	if (CP_Input_KeyTriggered(CP_KEY::KEY_W))
	{
		is_frame_waiting_ = !is_frame_waiting_;
	}

	auto* local_control = is_host_ ? &host_control_ : &non_host_control_;
	auto* remote_control = is_host_ ? &non_host_control_ : &host_control_;

	// if the frames are up to date (or we are not waiting):
	// 1) Update the simulation IF we are the host
	// 2) Send the current state
	if (!is_frame_waiting_ || (local_frame_ <= remote_frame_))
	{
		const bool is_local_paused = CP_Input_KeyDown(KEY_SPACE);
		if (is_host_)
		{
			if (!is_local_paused)
			{
				local_control->Update();
			}
			if (!is_remote_paused_)
			{
				remote_control->Update();
			}
		}

		packet_.Reset();
		PacketSerializer::WriteValue<u_long>(packet_, ++local_frame_);
		// the client only sends control updates, while the host sends all positions
		if (is_host_)
		{
			PacketSerializer::WriteValue<float>(packet_, host_control_.GetCurrentX());
			PacketSerializer::WriteValue<float>(packet_, host_control_.GetCurrentY());
			PacketSerializer::WriteValue<float>(packet_, non_host_control_.GetCurrentX());
			PacketSerializer::WriteValue<float>(packet_, non_host_control_.GetCurrentY());
		}
		else
		{
			PacketSerializer::WriteValue<bool>(packet_, is_local_paused);
		}

		send(socket_, packet_.GetRoot(), packet_.GetUsedSpace(), 0);
	}
	
	// if we are ahead of the remote, look for network data
	if (!is_frame_waiting_ || (remote_frame_ < local_frame_))
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
				// the host only receives control updates, while the client receives all positions
				if (is_host_)
				{
					PacketSerializer::ReadValue<bool>(packet_, is_remote_paused_);
				}
				else
				{
					float host_x, host_y, non_host_x, non_host_y;
					PacketSerializer::ReadValue<float>(packet_, host_x);
					PacketSerializer::ReadValue<float>(packet_, host_y);
					PacketSerializer::ReadValue<float>(packet_, non_host_x);
					PacketSerializer::ReadValue<float>(packet_, non_host_y);
					remote_player_.SetPosition(host_x, host_y);
					local_player_.SetPosition(non_host_x, non_host_y);
				}
			}
		}
	}

	if (is_host_)
	{
		local_player_.SetPosition(local_control->GetCurrentX(), local_control->GetCurrentY());
		remote_player_.SetPosition(remote_control->GetCurrentX(), remote_control->GetCurrentY());
	}
}


void DumbClientScenarioState::Draw()
{
	ScenarioState::Draw();

	local_player_.Draw();
	remote_player_.Draw();
}


std::string DumbClientScenarioState::GetDescription() const
{
	std::string description("DumbClient Scenario, ");
	description += is_host_ ? "Host, " : "Non-Host, ";
	description += "Local: ";
	description += std::to_string(local_frame_);
	description += ", Remote: ";
	description += std::to_string(remote_frame_);
	description += is_frame_waiting_ ? ", Waiting for Frame" : ", Not Waiting for Frame";
	return description;
}


std::string DumbClientScenarioState::GetInstructions() const
{
	return "Hold SPACE to halt the local (red) player. Press W to toggle frame-waiting";
}


bool DumbClientScenarioState::HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << "DumbClient WinSock Error: " << error_text << wsa_error << std::endl;

	// close the socket and clear it
	// -- this should trigger a GameStateManager reset in the next Update
	closesocket(socket_);
	socket_ = INVALID_SOCKET;

	return true;
}