//---------------------------------------------------------
// file:	ConnectingMenuState.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	The menu shown when connecting to a server
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "ConnectingMenuState.h"
#include "GameStateManager.h"
#include "PacketSerializer.h"

const int kNetwork_Buffer_Size = 1024;
const float kConnecting_Timeout_Secs = 3.0f;


ConnectingMenuState::ConnectingMenuState(NetworkedScenarioState::NetworkedScenarioStateCreator scenario_state_creator, std::string game_type, ClientConfiguration configuration)
	: scenario_state_creator_(scenario_state_creator),
	  game_type_(game_type),
	  configuration_(configuration),
	  connecting_socket_(INVALID_SOCKET),
	  connecting_timer_secs_(kConnecting_Timeout_Secs)
{
	operation_description_ = "Connecting to ";
	operation_description_ += std::to_string(configuration_.game_port);
	operation_description_ += ", waiting for response from host...  ";

	network_buffer_ = new char[kNetwork_Buffer_Size];

	// create a UDP socket for connecting to a scenario host
	connecting_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if ((connecting_socket_ == INVALID_SOCKET) &&
		HandleSocketError("Error creating socket: "))
	{
		return;
	}

	// make the socket non-blocking
	u_long nonblocking = 1;
	auto res = ioctlsocket(connecting_socket_, FIONBIO, &nonblocking);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error setting non-blocking state on socket: "))
	{
		return;
	}

	// set the UDP socket to reference the specified port on the local machine (127.0.0.1)
	SOCKADDR_IN connecting_address;
	memset(&connecting_address, 0, sizeof(connecting_address));
	connecting_address.sin_family = AF_INET;
	connecting_address.sin_port = htons(configuration_.game_port);
	res = inet_pton(AF_INET, "127.0.0.1", &connecting_address.sin_addr);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error creating a localhost address for the socket to connect to: "))
	{
		return;
	}
	res = connect(connecting_socket_, reinterpret_cast<SOCKADDR*>(&connecting_address), sizeof(connecting_address));
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error 'connect'ing socket: "))
	{
		return;
	}

	// send the initial connection request
	SendConnectionRequest();
}


ConnectingMenuState::~ConnectingMenuState()
{
	delete[] network_buffer_;
}


void ConnectingMenuState::Update()
{
	// if the user presses ESC, close the socket, which means...
	if (CP_Input_KeyTriggered(KEY_ESCAPE) && (connecting_socket_ != INVALID_SOCKET))
	{
		closesocket(connecting_socket_);
		connecting_socket_ = INVALID_SOCKET;
	}

	// if our socket is invalid, return to the main menu 
	if (connecting_socket_ == INVALID_SOCKET)
	{
		GameStateManager::ReturnToBaseState();
		return;
	}

	// reduce the timer by CP_System_GetDt(), and if expired, give up and move on to Hosting
	connecting_timer_secs_ -= CP_System_GetDt();
	if (connecting_timer_secs_ <= 0.0f)
	{		
		std::cout << "Timeout waiting for a response from a server on port " << configuration_.game_port << ", attempting to connect again..." << std::endl;
		SendConnectionRequest();
		connecting_timer_secs_ = kConnecting_Timeout_Secs;
	}

	// attempt to receive a response from a hosting server
	const auto res = recv(connecting_socket_, network_buffer_, kNetwork_Buffer_Size, 0);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error receiving on connection socket: "))
	{
		return;
	}

	// if any bytes are received, then check the response, and determine if we are accepted
	if (res > 0)
	{
		std::string server_response;
		Packet packet = Packet(network_buffer_, res);
		PacketSerializer::ReadString(packet, server_response);
		std::cout << "Received a response from a server on port " << configuration_.game_port << ", which was: " << server_response.c_str() << std::endl;

		// if it's the magic string, move on to the scenario
		if (server_response == "LetUsBegin")
		{
			std::cout << "Successfully connected, moving on to the " << game_type_.c_str() << " scenario..." << std::endl;
			auto* game_state = scenario_state_creator_(connecting_socket_, false);
			GameStateManager::ApplyState(game_state);
		}
		else
		{
			std::cout << "Something went wrong when connecting (code " << server_response << ") - perhaps a token or game mismatch? - giving up..." << std::endl;
			closesocket(connecting_socket_);
			connecting_socket_ = INVALID_SOCKET;
			// this will result in exiting the menu in the next Update
		}
	}
}

void ConnectingMenuState::Draw()
{
	// build the timer description
	const auto timer_description = "Trying again in " + std::to_string(connecting_timer_secs_) + " seconds...";

	// draw the descriptions
	CP_Settings_TextSize(30);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP);
	CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
	CP_Font_DrawText(operation_description_.c_str(), 0.0f, 0.0f);
	CP_Font_DrawText(timer_description.c_str(), 0.0f, 35.0f);
}


void ConnectingMenuState::SendConnectionRequest()
{
	//NOTE: in Assignment 4, we send more values here...
	Packet packet = Packet(network_buffer_, kNetwork_Buffer_Size);
	PacketSerializer::WriteString(packet, game_type_);
	
	// send the scenario-specific challenge message to the server, hoping for a response
	const auto res = send(connecting_socket_, packet.GetRoot(), packet.GetUsedSpace(), 0);
	if ((res == SOCKET_ERROR) &&
		HandleSocketError("Error sending challenge on socket: "))
	{
		return;
	}
	
	std::cout << "Attempting to connect to a scenario server on port " << game_type_ << std::endl;
}


bool ConnectingMenuState::HandleSocketError(const char* error_text)
{
	const auto wsa_error = WSAGetLastError();

	// ignore WSAEWOULDBLOCK
	if (wsa_error == WSAEWOULDBLOCK)
	{
		return false;
	}

	// we expect WSAECONNRESET - it means there's nobody hosting yet.  Wait for the retry...
	if (wsa_error == WSAECONNRESET)
	{
		std::cout << "Received WSAECONNRESET when attempting to connect to a server on port " << configuration_.game_port << ", ignoring..." << std::endl;
		return false;
	}

	// log unexpected errors and return to the default game mode
	std::cerr << "Connecting Winsock Error: " << error_text << wsa_error << std::endl;

	// close the socket and clear it
	// -- this should trigger a GameStateManager reset in the next Update
	closesocket(connecting_socket_);
	connecting_socket_ = INVALID_SOCKET;

	return true;
}