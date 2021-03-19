//---------------------------------------------------------
// file:	OptimisticAttack.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Represents an instant area-of-effect "attack" on the other player.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "OptimisticAttack.h"
#include "LabMath.h"

const float kTargetDrawSize = 20.0f;


OptimisticAttack::OptimisticAttack(const float attack_x, const float attack_y, const float target_x, const float target_y, const float radius, const SyncRatio& attack_sync)
{
    x_ = attack_x;
    y_ = attack_y;
    target_x_ = target_x;
    target_y_ = target_y;
    radius_ = radius;
    sync_ratio_ = attack_sync;
}


void OptimisticAttack::Draw()
{
    if (!IsVisible())
    {
        return;
    }

    CP_Settings_NoStroke();
    CP_Settings_Fill(CP_Color_Create(attack_color_.r, attack_color_.g, attack_color_.b, alpha_));
    CP_Graphics_DrawCircle(x_, y_, radius_ * 2.0f);

    CP_Settings_NoStroke();
    CP_Settings_Fill(CP_Color_Create(target_color_.r, target_color_.g, target_color_.b, 255));
    CP_Graphics_DrawCircle(target_x_, target_y_, kTargetDrawSize);

    if (is_confirmed_)
    {
        CP_Settings_NoStroke();
        CP_Settings_Fill(CP_Color_Create(confirmed_target_color_.r, confirmed_target_color_.g, confirmed_target_color_.b, 255));
        CP_Graphics_DrawCircle(confirmed_target_x_, confirmed_target_y_, kTargetDrawSize -4);
    }

    alpha_ -= 3;
}