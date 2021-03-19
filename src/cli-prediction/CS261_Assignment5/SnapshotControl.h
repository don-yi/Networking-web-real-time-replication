//---------------------------------------------------------
// file:	DeadReckoningControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Calculates the position as *interpolated* from the last two known positions
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include "RemoteControl.h"


/// <summary>
/// Calculates the position as *interpolated* from the last two known positions.
/// </summary>
class SnapshotControl final
	: public RemoteControl
{
public:
	struct State
	{
		float x = 0.0f;
		float y = 0.0f;
		float time_since_last_update_secs = 0.0f;
	};

	void AddSnapshot(const State& new_state, u_long remote_frame);
	float CalculateX(float t) const;
	float CalculateY(float t) const;

	void Update(float dt) override;
	void Draw() override;

	inline SyncRatio GetSyncRatio() const override { return sync_ratio_; }
	
private:
	State latest_state_, previous_state_;
	SyncRatio sync_ratio_;
	bool is_initialized_ = false;
};