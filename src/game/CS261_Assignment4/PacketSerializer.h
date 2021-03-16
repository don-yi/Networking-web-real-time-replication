//---------------------------------------------------------
// file:	PacketSerializer.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Provides support for reading and writing a few specific value types into a packet.
//
// remarks: This could be optimized in a variety of ways, including bit-packing, etc.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once
#include "pch.h"
#include "Packet.h"


namespace PacketSerializer
{
	bool ReadString(Packet& packet, std::string& text);
	bool WriteString(Packet& packet, const std::string& text);

	/// <summary>
	/// Read a value from the provided packet
	/// </summary>
/// <returns>If true, the data was successfully read out of the packet.</returns>
	template <typename T>
	bool ReadValue(Packet& packet, T& value)
	{
		const auto read_size = sizeof(T);

		// is the value size beyond the end of the safe read zone?
		if (packet.GetRemainingSpace() < read_size)
		{
			return false;
		}

		// read the value
		const auto* reader = reinterpret_cast<const T*>(packet.GetTarget());
		value = *reader;
		
		// advance the packet
		packet.Advance(read_size);

		return true;
	}

	/// <summary>
	/// Write a value into the provided packet.
	/// </summary>
    /// <returns>If true, the data was successfully written into the packet.</returns>
	template <typename T>
	bool WriteValue(Packet& packet, const T value)
	{
		const auto write_size = sizeof(T);

		// is the value size beyond the end of the safe read zone?
		if (packet.GetRemainingSpace() < write_size)
		{
			return false;
		}

		// write the value
		auto* writer = reinterpret_cast<T*>(packet.GetTarget());
		*writer = value;

		// advance the packet
		packet.Advance(write_size);

		return true;
	}
};