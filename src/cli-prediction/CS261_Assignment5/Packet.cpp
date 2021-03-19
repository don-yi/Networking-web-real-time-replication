//---------------------------------------------------------
// file:	Packet.cpp
// author:	Matthew Picioccio
// email:	matthew.picioccio@digipen.edu
//
// brief:	Encapsulates buffer logic for reading or writing multiple items out of a single buffer.
//
// Copyright © 2021 DigiPen, All rights reserved.
//---------------------------------------------------------
#include "pch.h"
#include "Packet.h"


Packet::Packet(unsigned int buffer_size)
	: buffer_size_(buffer_size),
	remaining_space_(buffer_size),
	owns_buffer_(true)
{
	target_ = buffer_ = new char[buffer_size_];
}


Packet::Packet(char* buffer, unsigned int buffer_size)
	: buffer_(buffer),
	buffer_size_(buffer_size),
	owns_buffer_(false),
	remaining_space_(buffer_size),
	target_(buffer)
{ }


Packet::~Packet()
{
	if (owns_buffer_)
	{
		delete[] buffer_;
		buffer_ = nullptr;
	}
}


bool Packet::Advance(unsigned int bytes_advanced)
{
	if (bytes_advanced > remaining_space_)
	{
		return false;
	}

	remaining_space_ -= bytes_advanced;
	target_ += bytes_advanced;

	return true;
}


void Packet::Reset()
{
	remaining_space_ = buffer_size_;
	target_ = buffer_;
}