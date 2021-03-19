//---------------------------------------------------------
// file:	SyncRatio.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Describes a ratio between two frames of data.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


/// <summary>
/// Describes a ratio between two frames of data.
/// </summary>
struct SyncRatio
{
	u_long base_frame;
	u_long target_frame;
	float t;
};