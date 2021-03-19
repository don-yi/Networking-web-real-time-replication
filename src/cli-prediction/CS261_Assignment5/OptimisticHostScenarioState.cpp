//---------------------------------------------------------
// file:	OptimisticHostScenarioState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The optimistic scenario for the host, in which the local system is the authority.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "OptimisticHostScenarioState.h"
#include "PacketSerializer.h"
#include "DoubleOrbitControl.h"
#include "LabMath.h"

const int kNetworkBufferSize = 1024; // maximum size of our packet
const float kAttackTextSize = 30.0f; // The size of the attack text.
const CP_Color kAttackTextColor = CP_Color_Create(255, 255, 255, 255); // The color of the attack text.
const int kLocalStateHistorySize = 100; // the amount of state records to keep



OptimisticHostScenarioState::OptimisticHostScenarioState(const SOCKET socket)
	: NetworkedScenarioState(socket, true),
	local_control_(200.0f, 250.0f, 100.0f, 1.5f),
	is_client_attack_confirm_queued_(false),
	local_frame_(0),
	remote_frame_(0),
	send_timer_secs_(0.0f),
	target_time_between_send_(0.0f),
	packet_(kNetworkBufferSize)
{
	remote_control_.SetPosition(250.0f, 250.0f);
	local_player_.color = CP_Color_Create(255, 0, 0, 255);
	remote_player_.color = CP_Color_Create(0, 0, 255, 255);
}


OptimisticHostScenarioState::~OptimisticHostScenarioState() = default;


void OptimisticHostScenarioState::Update()
{
	NetworkedScenarioState::Update();

	if (CP_Input_KeyTriggered(CP_KEY::KEY_W))
	{
		target_time_between_send_ += 0.1f;
		if (target_time_between_send_ > 0.5f)
		{
			target_time_between_send_ = 0.0f;
		}
	}

	const bool is_local_paused = CP_Input_KeyDown(KEY_SPACE);
	local_control_.Update(!is_local_paused ? dt : 0.0f);

	local_player_.SetPosition(local_control_.GetCurrentX(), local_control_.GetCurrentY());
	remote_player_.SetPosition(remote_control_.GetCurrentX(), remote_control_.GetCurrentY());

	packet_.Reset();
	const auto res = recv(socket_, packet_.GetRoot(), packet_.GetRemainingSpace(), 0);
	if (res > 0)
	{
		u_long received_frame;
		PacketSerializer::ReadValue<u_long>(packet_, received_frame);
		// only use data if it's newer than the last frame we received
		if (received_frame > remote_frame_)
		{
			remote_frame_ = received_frame;
			bool control_up, control_down, control_left, control_right;
			float control_dt;
			PacketSerializer::ReadValue<bool>(packet_, control_up);
			PacketSerializer::ReadValue<bool>(packet_, control_down);
			PacketSerializer::ReadValue<bool>(packet_, control_left);
			PacketSerializer::ReadValue<bool>(packet_, control_right);
			PacketSerializer::ReadValue<float>(packet_, control_dt);
			remote_control_.SetControls(control_up, control_down, control_left, control_right);
			remote_control_.Update(control_dt);

			bool is_client_attacking;
			PacketSerializer::ReadValue<bool>(packet_, is_client_attacking);
			if (is_client_attacking)
			{
				float attack_x, attack_y, radius;
				u_long base_attack_frame, target_attack_frame;
				float attack_t;
				PacketSerializer::ReadValue<float>(packet_, attack_x);
				PacketSerializer::ReadValue<float>(packet_, attack_y);
				PacketSerializer::ReadValue<float>(packet_, radius);
				PacketSerializer::ReadValue<u_long>(packet_, base_attack_frame);
				PacketSerializer::ReadValue<u_long>(packet_, target_attack_frame);
				PacketSerializer::ReadValue<float>(packet_, attack_t);
				// calculate historical position of the local (target) player, using remote sync information and stored frames
				const auto base_attack_record_iter = std::find_if(local_state_history_.begin(), local_state_history_.end(), [=](ControlStateRecord record) { return record.frame == base_attack_frame; });
				const auto target_attack_record_iter = std::find_if(local_state_history_.begin(), local_state_history_.end(), [=](ControlStateRecord record) { return record.frame == target_attack_frame; });
				if (base_attack_record_iter == local_state_history_.end())
				{
					std::cerr << "Base attack frame " << base_attack_frame << " was not found" << std::endl;
				}
				else if (target_attack_record_iter == local_state_history_.end())
				{
					std::cerr << "Base attack frame " << base_attack_frame << " found, BUT target attack frame " << target_attack_frame << " was not found" << std::endl;
				}
				else
				{
					//GOAL: calculate historical position of the local (target) player, using remote sync information and stored frames
					float target_x, target_y;
					SnapshotControl snapshot_control;
					snapshot_control.AddSnapshot(base_attack_record_iter->snapshot_state, base_attack_record_iter->frame);
					snapshot_control.AddSnapshot(target_attack_record_iter->snapshot_state, target_attack_record_iter->frame);
					target_x = snapshot_control.CalculateX(attack_t);
					target_y = snapshot_control.CalculateY(attack_t);
					client_attack_ = std::unique_ptr<OptimisticAttack>(new OptimisticAttack(attack_x, attack_y, target_x, target_y, radius, { base_attack_frame, target_attack_frame, attack_t }));
					is_client_attack_confirm_queued_ = true;
				}
			}
		}
	}

	send_timer_secs_ += dt;
	if (send_timer_secs_ >= target_time_between_send_)
	{
		packet_.Reset();
		PacketSerializer::WriteValue<u_long>(packet_, local_frame_);
		//TODO: write a u_long value into the host's packet, "remote_frame_", into the packet
		PacketSerializer::WriteValue<float>(packet_, send_timer_secs_);
		// CONVENTION: host writes its own values first
		PacketSerializer::WriteValue<float>(packet_, local_control_.GetCurrentX());
		PacketSerializer::WriteValue<float>(packet_, local_control_.GetCurrentY());
		PacketSerializer::WriteValue<float>(packet_, remote_control_.GetCurrentX());
		PacketSerializer::WriteValue<float>(packet_, remote_control_.GetCurrentY());

		PacketSerializer::WriteValue<bool>(packet_, client_attack_ && is_client_attack_confirm_queued_);
		if (client_attack_ && is_client_attack_confirm_queued_)
		{
			PacketSerializer::WriteValue<float>(packet_, client_attack_->GetTargetX());
			PacketSerializer::WriteValue<float>(packet_, client_attack_->GetTargetY());
			is_client_attack_confirm_queued_ = false;
		}
		send(socket_, packet_.GetRoot(), packet_.GetUsedSpace(), 0);
		send_timer_secs_ = 0.0f;

		local_state_history_.push_back({ local_frame_, local_control_.GetState(), {local_control_.GetCurrentX(), local_control_.GetCurrentY(), send_timer_secs_} });
		while (local_state_history_.size() > kLocalStateHistorySize)
		{
			local_state_history_.pop_front();
		}
	}

	++local_frame_;
}


void OptimisticHostScenarioState::Draw()
{
	ScenarioState::Draw();

	if (client_attack_ && client_attack_->IsVisible())
	{
		client_attack_->Draw();
		std::string attack_status;
		if (client_attack_->IsTargetHit())
		{
			local_player_.color = CP_Color_Create(255, 255, 0, 255);
			attack_status = "Client Attack: HIT";
		}
		else
		{
			local_player_.color = CP_Color_Create(255, 0, 0, 255);
			attack_status = "Client Attack: MISS";
		}
		CP_Settings_TextSize(kAttackTextSize);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
		CP_Settings_Fill(kAttackTextColor);
		CP_Font_DrawText(attack_status.c_str(), 0.0f, 715.0f);
	}
	else
	{
		local_player_.color = CP_Color_Create(255, 0, 0, 255);
	}

	local_player_.Draw();
	remote_player_.Draw();
}


std::string OptimisticHostScenarioState::GetDescription() const
{
	std::string description("Optimistic Scenario, Host, Local: ");
	description += std::to_string(local_frame_);
	description += ", Remote: ";
	description += std::to_string(remote_frame_);
	description += ", Send Target: ";
	description += std::to_string(static_cast<int>(target_time_between_send_ * 1000));
	description += "ms";
	return description;
}


std::string OptimisticHostScenarioState::GetInstructions() const
{
	return "Hold SPACE to halt the local (red) player, W to increase Send Target";
}


bool OptimisticHostScenarioState::HandleSocketError(const char* error_text)
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