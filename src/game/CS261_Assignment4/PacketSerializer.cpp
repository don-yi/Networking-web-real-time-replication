//---------------------------------------------------------
// file:	PacketSerializer.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Provides support for reading and writing a few specific value types into a packet.
//
// remarks: This could be optimized in a variety of ways, including bit-packing, etc.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "PacketSerializer.h"


/// <summary>
/// Read a string from the provided packet
/// </summary>
/// <returns>If true, the data was successfully read out of the packet.</returns>
bool PacketSerializer::ReadString(Packet& packet, std::string& text)
{
	auto text_size_size = sizeof(uint32_t);

	// is the string *size* beyond the end of the safe read zone?
	if (packet.GetRemainingSpace() < text_size_size)
	{
		return false;
	}

	// read the string size
	const auto* size_reader = reinterpret_cast<const uint32_t*>(packet.GetTarget());
	const auto text_size = *size_reader;

	// is the string beyond the end of the safe read zone?
	const auto string_size = text_size * sizeof(char);
	if (packet.GetRemainingSpace() < string_size)
	{
		return false;
	}

	// advance the packet, now that we know we are really reading a valid string...
	packet.Advance(text_size_size);

	// read in the text parameter
	const auto* read_text = reinterpret_cast<const char*>(packet.GetTarget());
	text = std::string(read_text, text_size);

	// advance the packet
	packet.Advance(string_size);

	return true;
}


/// <summary>
/// Write a string int the provided packet
/// </summary>
/// <returns>If true, the data was successfully written into the packet.</returns>
bool PacketSerializer::WriteString(Packet& packet, const std::string& text)
{
	const auto text_size_size = sizeof(uint32_t);
	const auto text_size = sizeof(char) * static_cast<uint32_t>(text.size());
	const auto write_size = text_size_size + text_size;

	// is the string size beyond the end of the safe writing zone (including the size value itself)?
	if (packet.GetRemainingSpace() < write_size)
	{
		return false;
	}

	// write the string length
	auto* size_writer = reinterpret_cast<uint32_t*>(packet.GetTarget());
	*size_writer = text_size;
	
	// advance the packet
	packet.Advance(text_size_size);

	// write the string itself
	auto* text_writer = reinterpret_cast<char*>(packet.GetTarget());
	for (const auto element : text)
	{
		*text_writer = element;
		++text_writer;
	}

	// advance the packet
	packet.Advance(text_size);

	return true;
}