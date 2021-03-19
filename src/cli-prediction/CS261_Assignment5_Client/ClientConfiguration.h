//---------------------------------------------------------
// file:	ClientConfiguration.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The app configuration, which comes from the command line and other potential sources.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "framework.h"


/// <summary>
/// The app configuration, which comes from the command line and other potential sources.
/// </summary>
struct ClientConfiguration
{
	int game_port = 4200;

	static ClientConfiguration BuildConfigurationFromArguments(int argc, char** argv);
};

