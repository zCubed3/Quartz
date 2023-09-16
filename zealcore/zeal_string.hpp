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
// zeal_string.hpp - zealString definition
//

#ifndef ZEALOT_ZEAL_STRING_HPP
#define ZEALOT_ZEAL_STRING_HPP

//============================================================================

#include <cstddef>

//============================================================================

// A custom string type, meant to replace std::string and provide a much simpler implementation
class zealString
{
protected:
	char* 	buffer 	= nullptr;
	size_t	size 	= 0; // How large is the buffer?
	size_t 	top 	= 0; // Where is the top most character (can't be greater than size!)

protected:
	// ==================
	//  Internal Methods
	// ==================
	void Reallocate(size_t new_size, bool release = true);

public:
	// ==============
	//  Construction
	// ==============
	~zealString();
	zealString();

	explicit zealString(size_t size);
	explicit zealString(const char* str);

	zealString(const zealString& str);

	// =========
	//  Methods
	// =========
	const char* 	CStr();

	// ===========
	//  Operators
	// ===========

	// TODO: Move these to the source file?

	char 			operator[](std::size_t index) const;

	zealString 		operator+(const zealString& rhs) const;

	zealString 		operator+=(const zealString& rhs);
};

//============================================================================

#endif//ZEALOT_ZEAL_STRING_HPP
