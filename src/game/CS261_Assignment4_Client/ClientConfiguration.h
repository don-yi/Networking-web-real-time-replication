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
	std::string user_service;
	std::string username;
	std::string password;
	std::string avatar;
	std::string token;
	int game_port;

	static ClientConfiguration BuildConfigurationFromArguments(int argc, char** argv);
};

