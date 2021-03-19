//---------------------------------------------------------
// file:	ServerConfiguration.h
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
struct ServerConfiguration
{
	int port;

	static ServerConfiguration BuildConfigurationFromArguments(int argc, char** argv);
};

