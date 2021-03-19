//---------------------------------------------------------
// file:	ServerConfiguration.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The app configuration, which comes from the command line and other potential sources.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "ServerConfiguration.h"


ServerConfiguration ServerConfiguration::BuildConfigurationFromArguments(int argc, char** argv)
{
    ServerConfiguration configuration;

    // argv[0] is the executable file name and path
    configuration.port = (argc > 1) ? atoi(argv[1]) : 4200;

    return configuration;
}