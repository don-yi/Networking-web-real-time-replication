//---------------------------------------------------------
// file:	HostingMenuState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when hosting a server, and waiting for a user
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "HostingMenuState.h"
#include "GameStateManager.h"
#include "PacketSerializer.h"

const int kNetwork_Buffer_Size = 1024;


HostingMenuState::HostingMenuState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ServerConfiguration configuration)
	: scenario_state_creator_(scenario_state_creator),
	  game_type_(game_type),
	  configuration_(configuration)
{
	operation_description_ = "Hosting on ";
	operation_description_ += std::to_string(configuration_.port);
	operation_description_ += ", waiting for connection...  ";

	network_buffer_ = new char[kNetwork_Buffer_Size];

	// create a UDP socket for connecting to a scenario host
	hosting_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ((hosting_socket_ == INVALID_SOCKET) &&
		HandleSocketError("Error creating socket: "))
	{
		return;
	}

	// make the socket non-blocking
	u_long nonblocking = 1;
	auto res = ioctlsocket(hosting_socket_, FIONBIO, &nonblocking);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error setting non-blocking state on socket: "))
	{
		return;
	}

	// bind the hosting socket to the specified port on the local machine (127.0.0.1)
	SOCKADDR_IN hosting_address;
	memset(&hosting_address, 0, sizeof(hosting_address));
	hosting_address.sin_family = AF_INET;
	hosting_address.sin_port = htons(configuration_.port);
	res = inet_pton(AF_INET, "127.0.0.1", &hosting_address.sin_addr);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error creating a localhost address for the socket to host on: "))
	{
		return;
	}
	res = bind(hosting_socket_, reinterpret_cast<SOCKADDR*>(&hosting_address), sizeof(hosting_address));
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error binding hosting socket: "))
	{
		return;
	}

	std::cout << "Hosting a server on port " << configuration_.port << std::endl;
}


HostingMenuState::~HostingMenuState()
{
	delete[] network_buffer_;
}


void HostingMenuState::Update()
{
	// if the user presses ESC, close the socket, which means...
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && (hosting_socket_ != INVALID_SOCKET))
	{
		closesocket(hosting_socket_);
		hosting_socket_ = INVALID_SOCKET;
	}

	// if our socket is invalid, return to the main menu 
	if (hosting_socket_ == INVALID_SOCKET)
	{
		GameStateManager::ReturnToBaseState();
		return;
	}

	// attempt to receive a message from a connecting client
	SOCKADDR_IN other_address;
	int other_address_size = sizeof(other_address);
	auto res = recvfrom(hosting_socket_, network_buffer_, kNetwork_Buffer_Size, 0, reinterpret_cast<SOCKADDR*>(&other_address),
		&other_address_size);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error receiving on hosting socket: "))
	{
		return;
	}

	// if any data is received:
	if (res > 0)
	{
		std::cout << "Received a message from a potential connection, accepting to the game..." << std::endl;

		//NOTE: in Assignment 4, there's actual token-validation logic here...

		// read the client data out of the packet
		Packet packet = Packet(network_buffer_, res);
		std::string client_game_type;
		PacketSerializer::ReadString(packet, client_game_type);

		if (game_type_ == client_game_type)
	{
			std::cout << "Game-type matched!  Continuing to game..." << std::endl;
			SendConnectionSuccess(other_address);
		}
		else
		{
			std::cout << "Game type mismatch: expected '" << game_type_ << "', received '" << client_game_type << "'.  Rejecting..." << std::endl;
			SendConnectionFailure(other_address, "BadGameType");
		}
	}
}


void HostingMenuState::Draw()
{
	// draw the description
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(operation_description_.c_str(), 0.0f, 0.0f);
}


bool HostingMenuState::HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << "Hosting Winsock Error: " << error_text << wsa_error << std::endl;

	// close the socket and clear it
	// -- this should trigger a GameStateManager reset in the next Update
	closesocket(hosting_socket_);
	hosting_socket_ = INVALID_SOCKET;

	return true;
}


void HostingMenuState::SendConnectionSuccess(SOCKADDR_IN other_address)
{
	// set the hosting socket to reference the address the message was received from
	auto res = connect(hosting_socket_, reinterpret_cast<SOCKADDR*>(&other_address), sizeof(other_address));
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error 'connect'ing on hosting socket: "))
	{
		return;
	}

	// send the magic success string to the client
	Packet packet = Packet(network_buffer_, kNetwork_Buffer_Size);
	PacketSerializer::WriteString(packet, "LetUsBegin");
	res = send(hosting_socket_, packet.GetRoot(), packet.GetUsedSpace(), 0);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error sending 'LetUsBegin' from hosting socket: "))
	{
		return;
	}

	// move on to the scenario, using the hosting socket, in host mode
	std::cout << "Successfully hosting a scenario on port " << configuration_.port << ", moving on to the scenario..." << std::endl;
	auto game_state = scenario_state_creator_(hosting_socket_, true);
	GameStateManager::ApplyState(game_state);
}


void HostingMenuState::SendConnectionFailure(SOCKADDR_IN other_address, const char* message)
{
	Packet packet = Packet(network_buffer_, kNetwork_Buffer_Size);
	PacketSerializer::WriteString(packet, message);
	auto res = sendto(hosting_socket_, packet.GetRoot(), packet.GetUsedSpace(), 0, (SOCKADDR*)&other_address, sizeof(other_address));
	if (res == SOCKET_ERROR)
	{
		HandleSocketError("Error sending 'GoAway' from hosting socket: ");
	}
}