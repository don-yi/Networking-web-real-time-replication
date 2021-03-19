#pragma once
#include "PlayerControl.h"
#include "SyncRatio.h"

class RemoteControl :
    public PlayerControl
{
public:
	virtual SyncRatio GetSyncRatio() const = 0;
};