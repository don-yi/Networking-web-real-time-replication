//---------------------------------------------------------
// file:	OptimisticAttack.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Represents an instant area-of-effect "attack" on the other player.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "cprocessing.h"
#include "SyncRatio.h"
#include "LabMath.h"


/// <summary>
/// Represents an instant area-of-effect "attack" on the other player.
/// </summary>
class OptimisticAttack
{
public:
	OptimisticAttack(float attack_x, float attack_y, float target_x, float target_y, float radius, const SyncRatio& attack_sync);

	void SetConfirmedTargetPosition(const float confirmed_target_x, const float confirmed_target_y) { confirmed_target_x_ = confirmed_target_x; confirmed_target_y_ = confirmed_target_y; is_confirmed_ = true; }

	void Draw();

	float GetAttackX() const { return x_; }
	float GetAttackY() const { return y_; }
	float GetTargetX() const { return target_x_; }
	float GetTargetY() const { return target_y_; }
	float GetRadius() const { return radius_; }
	SyncRatio GetSyncRatio() const { return sync_ratio_; }

	bool IsVisible() const { return alpha_ > 0; }
	bool IsTargetHit() const { return LabMath::IsWithinDistance(x_, y_, target_x_, target_y_, radius_); }
	bool IsTargetConfirmed() const { return is_confirmed_; }
	bool IsTargetHitConfirmed() { return is_confirmed_ ? LabMath::IsWithinDistance(x_, y_, confirmed_target_x_, confirmed_target_y_, radius_) : false; }

private:
	float x_ = 0.0f, y_ = 0.0f;
	float target_x_ = 0.0f, target_y_ = 0.0f;
	float radius_ = 0.0f;
	SyncRatio sync_ratio_{};

	bool is_confirmed_ = false;
	float confirmed_target_x_ = 0.0f, confirmed_target_y_ = 0.0f;

	int alpha_ = 255;
	CP_Color attack_color_{ 128, 128, 128 };
	CP_Color target_color_{ 255, 0, 255 };
	CP_Color confirmed_target_color_{ 255, 255, 255 };
};