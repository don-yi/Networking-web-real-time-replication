//---------------------------------------------------------
// file:	ClientConfiguration.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The app configuration, which comes from the command line and other potential sources.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "ClientConfiguration.h"


ClientConfiguration ClientConfiguration::BuildConfigurationFromArguments(int argc, char** argv)
{
    ClientConfiguration configuration;

    //NOTE: in Assignment 4, there are configuration values for the user service login process here...
    configuration.game_port = 4200;

    return configuration;
}