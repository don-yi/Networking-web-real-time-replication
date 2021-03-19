//---------------------------------------------------------
// file:	SimpleSyncControl.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Simply stores the last-known position and uses that.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "SimpleSyncControl.h"


void SimpleSyncControl::SetLastKnown(const float position_x, const float position_y, const u_long remote_frame)
{
	current_x_ = position_x;
	current_y_ = position_y;
	remote_frame_ = remote_frame;
}


SyncRatio SimpleSyncControl::GetSyncRatio() const
{
	const struct SyncRatio sync_ratio { remote_frame_, remote_frame_, 0.0f };
	return sync_ratio;
}