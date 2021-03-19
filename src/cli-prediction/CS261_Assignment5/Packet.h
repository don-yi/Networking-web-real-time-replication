//---------------------------------------------------------
// file:	Packet.h
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Encapsulates buffer logic for reading or writing multiple items out of a single buffer.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#pragma once


/// <summary>
/// Encapsulates buffer logic for reading or writing multiple items out of a single buffer.
/// </summary>
class Packet
{
public:
	Packet(unsigned int buffer_size);
	Packet(char* buffer, unsigned int buffer_size);
	~Packet();

	unsigned int GetUsedSpace() inline const { return (buffer_size_ > remaining_space_) ? buffer_size_ - remaining_space_ : 0; }
	unsigned int GetRemainingSpace() inline const { return remaining_space_; }
	char* GetTarget() inline const { return target_; }
	char* GetRoot() inline const { return buffer_; }

	bool Advance(unsigned int bytes_advanced);
	void Reset();

private:
	char* buffer_;
	unsigned int buffer_size_;
	bool owns_buffer_;

	unsigned int remaining_space_;
	char* target_;
};