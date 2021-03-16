//---------------------------------------------------------
// file:	DigestBuilder.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Builds encoded digests using SHA256 and OpenSSL
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "framework.h"


/// <summary>
/// Builds encoded digests using SHA256 and OpenSSL
/// </summary>
/// <remarks>This technique is intended to parallel the token encoding in the CS 261 user service.</remarks>
namespace DigestBuilder
{
    std::string BuildDigest(const std::string& message);
};