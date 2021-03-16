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

    // argv[0] is the executable file name and path
    configuration.user_service = (argc > 1) ? std::string(argv[1]) : "http://localhost:3100";
    configuration.username = (argc > 2) ? std::string(argv[2]) : "test_user";
    configuration.password = (argc > 3) ? std::string(argv[3]) : "test_password";

    return configuration;
}