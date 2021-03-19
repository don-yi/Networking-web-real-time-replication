//---------------------------------------------------------
// file:	SimpleSyncControl.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Simply stores the last-known position and uses that.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include "RemoteControl.h"


/// <summary>
/// Simply stores the last-known position and uses that.
/// </summary>
class SimpleSyncControl final
	: public RemoteControl
{
public:
	void SetLastKnown(float position_x, float position_y, u_long remote_frame);

	void Update(float dt) override { }
	void Draw() override { }

	SyncRatio GetSyncRatio() const override;
	
private:
	u_long remote_frame_ = 0;
};