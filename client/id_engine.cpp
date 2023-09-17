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

//============================================================================

#include "id_engine.hpp"

#include "console/id_cvar.hpp"

//============================================================================

idEngine* id_engine = new idEngine();

//============================================================================

idCvar* idEngine::SetCvarString(const zealString &name, const zealString &string)
{
	idCvar* cvar;

	// Ensure the Cvar exists
	cvar = GetCvar(name);

	// TODO: Why would a cvar be nullptr?
	if (cvar != nullptr)
		cvar->SetString(value);

	return cvar;
}

idCvar* idEngine::SetCvarValue(const zealString &name, float value)
{
	idCvar* cvar;

	// Ensure the Cvar exists
	cvar = GetCvar(name);

	// TODO: Why would a cvar be nullptr?
	if (cvar != nullptr)
		cvar->SetValue(value);

	return cvar;
}

// TODO: Handle Cvar flags
idCvar* idEngine::GetCvar(const zealString& name)
{
	idCvar* 							cvar;
	zealLinkedList<idCvar>::node_t*		node;

	cvar = nullptr;

	// TODO: Thread safety for the future?
	// TODO: Case insensitive comparison?
	for (node = cvar_list.Top(); node != nullptr; node = node->next)
		if (node->element.GetName() == name)
			return &node->element;

	// If we've made it here it means we haven't found a match!
	cvar = new idCvar(name, 0);
}

//============================================================================