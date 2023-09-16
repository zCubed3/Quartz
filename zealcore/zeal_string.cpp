/*
Copyright (C) 2023 zCubed3 (Liam R.)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

//
// zeal_string.cpp - zealString implementation
//

#include "zeal_string.hpp"

//============================================================================

#include <cstring>

//============================================================================

// ==================
//  Internal Methods
// ==================
void zealString::Reallocate(size_t new_size, bool release)
{
	char*	old_buffer;
	size_t 	old_size;
	size_t 	real_size;
	size_t 	copy_size;

	old_buffer = this->buffer;
	old_size = this->size;
	real_size = new_size;

	// Is top out of range?
	// If so, make the effective size +1
	// And reduce our copy to only the slice we can fit
	if (top > new_size)
	{
		top = new_size;
		real_size += 1;

		copy_size = new_size;
	}
	else
		copy_size = top;

	this->size = real_size;
	this->buffer = new char[real_size];

	if (old_buffer != nullptr)
		strncpy(buffer, old_buffer, copy_size);

	if (release)
		delete[] old_buffer;
}

//============================================================================

// ==============
//  Construction
// ==============

zealString::~zealString()
{
	delete[] buffer;
}

zealString::zealString()
{
	// Allocate a 256 character buffer
	this->size = 256;
	this->buffer = new char[size];
}

zealString::zealString(size_t size)
{
	this->buffer = new char[size];
	this->size = size;
	// Top remains at zero
}

zealString::zealString(const char* str)
{
	// The top is equal to the last character of the old string
	top = strlen(str);

	// Append one more character in size to ensure termination
	size = top + 1;

	// Actually allocate
	buffer = new char[size];
	strcpy(buffer, str);
}

zealString::zealString(const zealString& str)
{
	// Copy size data
	this->top = str.top;
	this->size = str.size;

	// Allocate new buffer and copy it over
	this->buffer = new char[this->size];
	strcpy(this->buffer, str.buffer);
}

//============================================================================

// =========
//  Methods
// =========
const char* zealString::CStr()
{
	return buffer;
}

//============================================================================

char zealString::operator[](std::size_t index) const
{
	// TODO: Error when out of range
	if (index < size && buffer != nullptr)
		return buffer[index];

	return '\0';
}

zealString zealString::operator+(const zealString &rhs) const
{
	zealString 	ret_str;
	size_t 		combo_size;
	char 		*lower, *upper;

	combo_size = this->top + rhs.top;
	ret_str.top = combo_size;

	// Check if the combo exceeds the default size
	// Greater or eq because we need to validate that we don't go out of bounds
	if (combo_size >= ret_str.size)
		ret_str = zealString(combo_size + 1);

	// Copy the two strings
	lower = ret_str.buffer;
	upper = ret_str.buffer + this->top;

	strncpy(lower, this->buffer, this->top);
	strncpy(upper, rhs.buffer, rhs.top);

	return ret_str;
}

zealString zealString::operator+=(const zealString& rhs)
{
	size_t 		combo_size;
	char 		*upper;

	combo_size = this->top + rhs.top;

	// If our combo size has gone way over our normal size, reallocate (in place, keeping old data in new copy)
	// Our new size is two times the combo size to ensure subsequent additions are not causing many tiny reallocations
	if (combo_size >= this->size)
		Reallocate(combo_size * 2);

	// We only care about the upper half of our new string
	upper = this->buffer + this->top;

	strncpy(upper, rhs.buffer, rhs.top);
	this->top += rhs.top;

	return *this;
}