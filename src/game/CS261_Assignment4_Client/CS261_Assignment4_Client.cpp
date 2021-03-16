//---------------------------------------------------------
// file:	CS261_Assignment4_Client.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Entry point
//
// C-Processing documentation link:
// https://inside.digipen.edu/main/GSDP:GAM100/CProcessing
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "framework.h"
#include "GameStateManager.h"
#include "ClientMainMenuState.h"
#include "ClientConfiguration.h"


/// <summary>
/// Run as part of the very first engine-init call.
/// </summary>
/// <remarks>This happens just once, and after the window is created.</remarks>
void InitializeWindow()
{
	CP_System_SetWindowTitle("CS 261 Application 4: CLIENT");
}


/// <summary>
/// Enable the console, so we can see useful cout and cerr output.
/// For reference, see one of the newer answers in https://stackoverflow.com/questions/311955/redirecting-cout-to-a-console-in-windows.
/// </summary>
/// <remarks>In modern Windows SDKs, the old freopen method is not sufficient to capture cout/cerr.  This method does work.</remarks>
void ShowConsole()
{
	AllocConsole();

	static std::ofstream conout("CONOUT$", std::ios::out);
	std::cout.rdbuf(conout.rdbuf());
	std::cerr.rdbuf(conout.rdbuf());

	// NOTE: CP_Engine_Run will call FreeConsole() for us when the program is exiting.
}


/// <summary>
/// Entry point for the process
/// </summary>
/// <returns>0 if successful, non-zero for process errors</returns>
int main(const int argc, char** argv)
{
	auto configuration = ClientConfiguration::BuildConfigurationFromArguments(argc, argv);

	// initialize WinSock
	WSADATA wsa_data;
	const auto res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (res != 0)
	{
		std::cerr << "Error in WSAStartup: " << WSAGetLastError() << std::endl;
		return 1;
	}

	ShowConsole();

	// establish the initial window settings
	CP_System_SetWindowSize(1024, 768);

	// start the simulation
	auto* main_menu_state = new ClientMainMenuState(configuration);
	GameStateManager::Establish(main_menu_state, InitializeWindow);
	CP_Engine_Run();

	// clean up WinSock
	WSACleanup();

	return 0;
}
