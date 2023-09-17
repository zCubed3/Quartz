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

// ==============
//  Construction
// ==============

zealString::~zealString()
{
	delete[] buffer;
}

zealString::zealString()
{
	// Does nothing, should this do something?
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

zealString::zealString(const char* str, size_t size)
{
	size_t	actual_top;

	// Top is either the last character or size - 1
	actual_top = strlen(str);

	if (size < actual_top)
		actual_top = size - 1;

	this->size = size;
	this->top = actual_top;
	this->buffer = new char[size];

	strncpy(this->buffer, str, actual_top);
}

zealString::zealString(const zealString& str)
{
	// Copy size data
	this->top = str.top;
	this->size = str.size;

	// Allocate new buffer and copy it over
	this->buffer = new char[this->size];
	strncpy(this->buffer, str.buffer, str.size);
}

//============================================================================

// =========
//  Methods
// =========
char* zealString::Data()
{
	return buffer;
}

const char* zealString::CStr() const
{
	return buffer;
}

size_t zealString::Length() const
{
	return top;
}

size_t zealString::Capacity() const
{
	return size;
}

void zealString::Resize(size_t new_size)
{
	char*	old_buffer;
	size_t 	real_size;
	size_t 	copy_size;

	old_buffer = this->buffer;
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

	// Zero out the rest
	for (size_t t = top; t < size; t++)
		buffer[t] = '\0';

	delete[] old_buffer;
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
	zealString 	ret_str; // Uninitialized!
	size_t 		final_size;
	char		*write_dst;

	final_size = this->top + rhs.top;

	// Check if the combo exceeds our current size or is equal to it
	// Therefore reallocate of double the current size
	if (final_size >= this->size)
		ret_str = zealString(this->buffer, final_size * 2);
	else // Otherwise, just copy our string (entirely, size and all)
		ret_str = zealString(*this);

	write_dst = ret_str.buffer + this->top;

	strncpy(write_dst, rhs.buffer, rhs.top);
	ret_str.top = final_size;

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
		Resize(combo_size * 2);

	// We only care about the upper half of our new string
	upper = this->buffer + this->top;

	strncpy(upper, rhs.buffer, rhs.top);
	this->top += rhs.top;

	return *this;
}


zealString zealString::operator+(const char* rhs) const
{
	zealString 	ret_str; // Uninitialized!
	size_t 		str_size;
	size_t 		final_size;
	char		*write_dst;

	if (rhs == nullptr)
		return zealString(*this);

	str_size = strlen(rhs);
	final_size = this->top + str_size;

	// Check if the combo exceeds our current size or is equal to it
	// Therefore reallocate of double the current size
	if (final_size >= this->size)
		ret_str = zealString(this->buffer, final_size * 2);
	else // Otherwise, just copy our string (entirely, size and all)
		ret_str = zealString(*this);

	write_dst = ret_str.buffer + this->top;

	strncpy(write_dst, rhs, str_size);
	ret_str.top = final_size;

	return ret_str;
}

zealString zealString::operator+=(const char* rhs)
{
	size_t 		combo_size;
	size_t 		str_size;
	char 		*upper;

	if (rhs == nullptr) // Don't append
		return *this;

	str_size = strlen(rhs);
	combo_size = this->top + str_size;

	// If our combo size has gone way over our normal size, reallocate (in place, keeping old data in new copy)
	// Our new size is two times the combo size to ensure subsequent additions are not causing many tiny reallocations
	if (combo_size >= this->size)
		Resize(combo_size * 2);

	// We only care about the upper half of our new string
	upper = this->buffer + this->top;

	strncpy(upper, rhs, str_size);
	this->top += str_size;

	return *this;
}

zealString zealString::operator+(const char &rhs) const
{
	zealString 	ret_str; // Uninitialized!
	size_t 		final_size;

	final_size = this->top + 1;

	// Check if the combo exceeds our current size or is equal to it
	// Therefore allocate a new string, double the current size
	if (final_size >= this->size)
		ret_str = zealString(this->buffer, final_size * 2);
	else // Otherwise, just copy our string (entirely, size and all)
		ret_str = zealString(*this);

	ret_str.buffer[final_size] = rhs;
	ret_str.top = final_size;

	return ret_str;
}

zealString zealString::operator+=(const char& rhs)
{
	size_t 		final_size;

	final_size = this->top + 1;

	// If our combo size has gone way over our normal size, reallocate (in place, keeping old data in new copy)
	// Our new size is two times the combo size to ensure subsequent additions are not causing many tiny reallocations
	if (final_size >= this->size)
		Resize(final_size * 2);

	this->buffer[this->top] = rhs;
	this->top += 1;

	return *this;
}

zealString& zealString::operator=(const zealString &rhs)
{
	// Check if we have enough space to insert the other string
	// If we don't, then replace the buffer in place
	if (rhs.size > size)
	{
		delete[] this->buffer;

		this->buffer = new char[rhs.size];
		this->size = rhs.size;
		this->top = rhs.top;

		strcpy(this->buffer, rhs.buffer);
	}
	else // Otherwise, replace our buffer contents with no reallocation!
	{
		size_t 	old_top;

		old_top	= this->top;
		this->top = rhs.top;

		// Zero out the remaining part
		for (size_t t = old_top; t < this->top; t++)
			this->buffer[t] = '\0';

		// Then copy
		strcpy(this->buffer, rhs.buffer);
	}

	return *this;
}
