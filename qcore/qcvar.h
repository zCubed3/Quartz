/*
Copyright (C) 1997-2001 Id Software, Inc., 2023 zCubed3 (Liam R.)

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
// qcvar.h - CVar declaration (here because it is uniform across the entire engine)
//

#ifndef ZEALOT_QCVAR_H
#define ZEALOT_QCVAR_H

//============================================================================

typedef enum
{
	CVAR_ARCHIVE 	= 1,
	CVAR_USERINFO 	= 2,
	CVAR_SERVERINFO = 4,
	CVAR_NOSET 		= 8,
	CVAR_LATCH 		= 16
} cvar_flags_t;

//============================================================================

// nothing outside the Cvar_*() functions should modify these fields!
typedef struct cvar_s
{
	char			*name;
	char			*string;
	char			*latched_string;	// for CVAR_LATCH vars
	cvar_flags_t	flags;
	qboolean		modified;	// set each time the cvar is changed
	float			value;
	struct cvar_s 	*next;
} cvar_t;

//============================================================================

#endif//ZEALOT_QCVAR_H
