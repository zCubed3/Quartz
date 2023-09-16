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
// zeal_misc.hpp - Misc functionality / classes
//

#ifndef ZEALOT_ZEAL_MISC_HPP
#define ZEALOT_ZEAL_MISC_HPP

//============================================================================

template<typename T>
struct zealIsPointer
{
	static constexpr bool value = false;
};

template<typename T>
struct zealIsPointer<T*>
{
	static constexpr bool value = true;
};

//============================================================================

template<typename T>
void zealOptionalDelete(const T& val) {}; // This is a value, not a pointer

template<typename T>
void zealOptionalDelete(T* val) // This is a pointer, delete it!
{
	delete val;
}

//============================================================================

#endif //ZEALOT_ZEAL_MISC_HPP