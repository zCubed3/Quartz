/*
Copyright (C) 1997-2001 Id Software, Inc.
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
// id_engine.cpp -- idEngine implementation (shared code between client, server, and imported modules)
//

#ifndef ZEALOT_ID_ENGINE_HPP
#define ZEALOT_ID_ENGINE_HPP

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

//============================================================================

#include "../zealcore/zeal_string.hpp"
#include "../zealcore/zeal_stack.hpp"

//============================================================================

class idCvar;

//============================================================================

class idEngine
{
	// =================
	//  Cvar Management
	// =================
public:
	zealStack<idCvar> 	cvar_list;

	// Sets the string of a cvar by name
	idCvar* 	SetCvarString(const zealString& name, const zealString& string);

	// Sets the value of a cvar by name
	idCvar* 	SetCvarValue(const zealString& name, float value);

	// Attempts to find a cvar by name, only returning it if it finds it!
	// You're left to update the value afterward!
	idCvar* 	GetCvar(const zealString& name);

	// TODO: Pass flags to both
	idCvar*		GetOrInitCvar(const zealString& name, const zealString& string);
	idCvar*		GetOrInitCvar(const zealString& name, float value);
};

//============================================================================

extern idEngine* 	id_engine;

//============================================================================

#endif //ZEALOT_ID_ENGINE_HPP
