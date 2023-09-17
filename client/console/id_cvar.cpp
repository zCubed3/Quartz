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
// id_cvar.cpp - idCvar implementation
//

//============================================================================

#include <cstdio>
#include <cstdlib>

#include "id_cvar.hpp"

//============================================================================

idCvar::idCvar(const zealString& name, const zealString& string, cvar_flags_t)
{
	this->name = name;

	SetString(string);
}

idCvar::idCvar(const zealString& name, float value, cvar_flags_t)
{
	this->name = name;

	SetValue(value);
}

//============================================================================

void idCvar::SetString(const zealString &string)
{
	this->string = string;
	this->value = atof(this->string.CStr());

	this->modified = true;
}

void idCvar::SetValue(float value)
{
	// Ensure we have enough space for our formatted string
	if (this->string.Capacity() < 64)
		this->string.Resize(64);

	sprintf(this->string.Data(), "%f", value);

	this->value = value;

	this->modified = true;
}

void idCvar::UnflagModified()
{
	this->modified = false;
}

qboolean idCvar::IsModified() const
{
	return this->modified;
}

//============================================================================