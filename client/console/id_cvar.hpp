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
// id_cvar.hpp - idCvar definition
//

#ifndef ZEALOT_ID_CVAR_HPP
#define ZEALOT_ID_CVAR_HPP

//============================================================================

#include "../../zealcore/zeal_string.hpp"

#include "../../qcore/qtypes.h"

//============================================================================

typedef enum
{
	CVAR_NONE		= 0,
	CVAR_ARCHIVE 	= 1 << 0,
	CVAR_USERINFO 	= 1 << 1,
	CVAR_SERVERINFO = 1 << 2,
	CVAR_NOSET 		= 1 << 3,
	CVAR_LATCH 		= 1 << 4
} cvar_flags_t;

//============================================================================

// A variable that's programmatically accessible but also user accessible
class idCvar
{
protected:
	zealString 		name;
	zealString 		string;

	float			value 		= 0.0F; // Updated when string is set, is atof result!
	qboolean		modified 	= false;

public:
	// ==============
	//  Construction
	// ==============
	idCvar(const zealString& name, const zealString& string, cvar_flags_t flags = CVAR_NONE);
	idCvar(const zealString& name, float value, cvar_flags_t = CVAR_NONE);

	// =========
	//  Methods
	// =========
	zealString 	GetName() const;

	void		SetString(const zealString& string);
	void 		SetValue(float value);

	// Removes the modified flag
	void 		UnflagModified();

	qboolean	IsModified() const;
};

//============================================================================

#endif //ZEALOT_ID_CVAR_HPP
