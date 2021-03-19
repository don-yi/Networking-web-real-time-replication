//---------------------------------------------------------
// file:	OptimisticClientScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario for the client, in which the remote is the authority.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "OptimisticClientScenarioState.h"
#include "PacketSerializer.h"
#include "Clock.h"

const int kNetworkBufferSize = 1024; // maximum size of our packet
const int kMaxMoveHistorySize = 100; // the maximum number of move records that can be stored in the history
const float kMouseAttackRadius = 50.0f; // the radius of the attack instigated by a mouse click
const float kTimeBetweenClientSend_Secs = 0.0f; // throttling the rates of sending updates from this process (the client) - SET TO 0 (DISABLED) FOR THIS ASSIGNMENT (see below)
const float kAttackTextSize = 30.0f; // The size of the attack text.
const CP_Color kAttackAgreeTextColor = CP_Color_Create(255, 255, 255, 255); // The color of the attack text when local and remote agree
const CP_Color kAttackDisagreeTextColor = CP_Color_Create(255, 0, 255, 255); // The color of the attack text when local and remote disagree.


OptimisticClientScenarioState::OptimisticClientScenarioState(const SOCKET socket)
	: NetworkedScenarioState(socket, false),
	is_drawing_controls_(false),
	is_attack_queued_(false),
	local_frame_(0),
	remote_frame_(0),
	send_timer_secs_(0.0f),
	time_since_last_recv_(0.0f),
	packet_(kNetworkBufferSize)
{
	direct_local_control_.SetPosition(250.0f, 250.0f);
	local_player_.color = CP_Color_Create(255, 0, 0, 255);
	remote_player_.color = CP_Color_Create(0, 0, 255, 255);
}


OptimisticClientScenarioState::~OptimisticClientScenarioState() = default;


void OptimisticClientScenarioState::Update()
{
	NetworkedScenarioState::Update();

	if (CP_Input_KeyTriggered(CP_KEY::KEY_Q))
	{
		is_drawing_controls_ = !is_drawing_controls_;
	}

	auto control_up = CP_Input_KeyDown(CP_KEY::KEY_W) == TRUE;
	auto control_down = CP_Input_KeyDown(CP_KEY::KEY_S) == TRUE;
	auto control_left = CP_Input_KeyDown(CP_KEY::KEY_A) == TRUE;
	auto control_right= CP_Input_KeyDown(CP_KEY::KEY_D) == TRUE;
	direct_local_control_.SetControls(control_up, control_down, control_left, control_right);

	//TODO: retrieve last frame's control values (x and y), i.e. *before* updating
	direct_local_control_.Update(dt);
	// retrieve this frame's control values (x and y), i.e. *after* updating
	auto direct_local_x = direct_local_control_.GetCurrentX();
	auto direct_local_y = direct_local_control_.GetCurrentY();
	// set the player (rendered) position based on the current control location
	local_player_.SetPosition(direct_local_x, direct_local_y);
	//TODO: calculate the delta (x and y) between this frame and last frame
	//TODO: push a new movement record onto the *back* of the history deque
	//TODO: while the size of the history deque is greater than kMaxMoveHistorySize, pop entries off the *front* of the deque

	snapshot_remote_control_.Update(dt);
	auto snapshot_remote_x = snapshot_remote_control_.GetCurrentX();
	auto snapshot_remote_y = snapshot_remote_control_.GetCurrentY();
	auto current_remote_sync = snapshot_remote_control_.GetSyncRatio();
	remote_player_.SetPosition(snapshot_remote_x, snapshot_remote_y);

	if (CP_Input_MouseClicked())
	{
		local_attack_ = std::unique_ptr<OptimisticAttack>(new OptimisticAttack(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY(), snapshot_remote_x, snapshot_remote_y, kMouseAttackRadius, current_remote_sync));
		is_attack_queued_ = true;
	}

	packet_.Reset();
	time_since_last_recv_ += dt;
	const auto res = recv(socket_, packet_.GetRoot(), packet_.GetRemainingSpace(), 0);
	if (res > 0)
	{
		u_long received_frame;
		PacketSerializer::ReadValue<u_long>(packet_, received_frame);
		// only use data if it's newer than the last frame we received
		if (received_frame > remote_frame_)
		{
			remote_frame_ = received_frame;
			u_long last_received_local_frame;
			float host_time_between_updates;
			float host_x, host_y;
			float non_host_x, non_host_y;
			// CONVENTION: host writes its own values first
			//TODO: read a u_long from the host's packet, into "last_received_local_frame"
			PacketSerializer::ReadValue<float>(packet_, host_time_between_updates);
			PacketSerializer::ReadValue<float>(packet_, host_x);
			PacketSerializer::ReadValue<float>(packet_, host_y);
			snapshot_remote_control_.AddSnapshot({ host_x, host_y, host_time_between_updates }, remote_frame_);

			// snap to the remote position
			// -- with TODOs, will add subsequent local moves
			PacketSerializer::ReadValue<float>(packet_, non_host_x);
			PacketSerializer::ReadValue<float>(packet_, non_host_y);
			direct_local_control_.SetLastRemotePosition(non_host_x, non_host_y); // for debug rendering
			//TODO: find the record with the same frame as last_received_local_frame
			// -- you likely should use std::find_if... see the implementation for server-rewind in OptimisticHostScenarioState.cpp for a hint on syntax!
			// -- if the record isn't found, you should describe the problem in cerr... but this is very unlikely 
			//TODO: advance the iterator one - we want the *next* delta
			//TODO: while (iterator != deque.end()), accumulate the subsequent movement deltas:
			// 1) add the record's delta (x and y) to the non_host_x and non_host_y
			// 2) advance the iterator
			// REMEMBER: ++ will advance iterators, you can get values from the iter using -> (i.e. iter->delta_x)
			direct_local_control_.SetPosition(non_host_x, non_host_y);

			// if there is a confirmed client attack, then process it
			auto is_attack_confirmed = false;
			PacketSerializer::ReadValue<bool>(packet_, is_attack_confirmed);
			if (is_attack_confirmed)
			{
				float confirmed_target_x, confirmed_target_y;
				PacketSerializer::ReadValue<float>(packet_, confirmed_target_x);
				PacketSerializer::ReadValue<float>(packet_, confirmed_target_y);
				if (local_attack_)
				{
					local_attack_->SetConfirmedTargetPosition(confirmed_target_x, confirmed_target_y);
				}
				else
				{
					std::cerr << "Received a confirmation for a non-existent attack... what happened??" << std::endl;
				}
			}
		}
		time_since_last_recv_ = 0.0f;
	}

	// NOTE: for this assignment, the send interval - kTimeBetweenClientSend_Secs - must be 0
	// -- if it wasn't, then the server would not have reasonably precise control updates, leading to a lot of corrections!
	// -- the fix for this would be to provide *all* of the missing sets (controls + dt) to the server (ex: 3ish at 100ms send interval)
	send_timer_secs_ += dt;
	if (send_timer_secs_ >= kTimeBetweenClientSend_Secs)
	{
		packet_.Reset();
		PacketSerializer::WriteValue<u_long>(packet_, local_frame_);
		PacketSerializer::WriteValue<bool>(packet_, control_up);
		PacketSerializer::WriteValue<bool>(packet_, control_down);
		PacketSerializer::WriteValue<bool>(packet_, control_left);
		PacketSerializer::WriteValue<bool>(packet_, control_right);
		PacketSerializer::WriteValue<float>(packet_, dt);

		PacketSerializer::WriteValue<bool>(packet_, local_attack_ && is_attack_queued_);
		if (local_attack_ && is_attack_queued_)
		{
			PacketSerializer::WriteValue<float>(packet_, local_attack_->GetAttackX());
			PacketSerializer::WriteValue<float>(packet_, local_attack_->GetAttackY());
			PacketSerializer::WriteValue<float>(packet_, local_attack_->GetRadius());
			const auto attack_sync = local_attack_->GetSyncRatio(); // use the stored sync, not the current one!
			PacketSerializer::WriteValue<u_long>(packet_, attack_sync.base_frame);
			PacketSerializer::WriteValue<u_long>(packet_, attack_sync.target_frame);
			PacketSerializer::WriteValue<float>(packet_, attack_sync.t);
			is_attack_queued_ = false;
		}
		send(socket_, packet_.GetRoot(), packet_.GetUsedSpace(), 0);
		send_timer_secs_ = 0.0f;
	}

	++local_frame_;
}


void OptimisticClientScenarioState::Draw()
{
	ScenarioState::Draw();

	remote_player_.color = CP_Color_Create(0, 0, 255, 255);
	if (local_attack_ && local_attack_->IsVisible())
	{
		local_attack_->Draw();
		std::string attack_status = local_attack_->IsTargetHit() ? "Local Attack: HIT" : "Local Attack: MISS";
		CP_Settings_Fill(kAttackAgreeTextColor);
		if (local_attack_->IsTargetConfirmed())
		{
			if (local_attack_->IsTargetHitConfirmed())
			{
				remote_player_.color = CP_Color_Create(255, 255, 0, 255);
				attack_status += ", Remote Confirm: HIT";
				CP_Settings_Fill(kAttackDisagreeTextColor);
			}
			else
			{
				attack_status += ", Remote Confirm: MISS";
			}
		}
		CP_Settings_TextSize(kAttackTextSize);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Font_DrawText(attack_status.c_str(), 0.0f, 715.0f);
	}

	local_player_.Draw();
	remote_player_.Draw();

	// draw the debug visualizations of our replication controls
	if (is_drawing_controls_)
	{
		snapshot_remote_control_.Draw();
		direct_local_control_.Draw();
	}
}


std::string OptimisticClientScenarioState::GetDescription() const
{
	std::string description("Optimistic Scenario, Client, Local: ");
	description += std::to_string(local_frame_);
	description += ", Remote: ";
	description += std::to_string(remote_frame_);
	return description;
}


std::string OptimisticClientScenarioState::GetInstructions() const
{
	return "Hold WASD to move local (red) player, click to attack, Q to toggle drawing";
}


bool OptimisticClientScenarioState::HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << "Optimistic WinSock Error: " << error_text << wsa_error << std::endl;

	// close the socket and clear it
	// -- this should trigger a GameStateManager reset in the next Update
	closesocket(socket_);
	socket_ = INVALID_SOCKET;

	return true;
}