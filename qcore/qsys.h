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
// qsys.h - Common system functions
//

#ifndef ZEALOT_QSYS_H
#define ZEALOT_QSYS_H

//============================================================================

// time returned by last Sys_Milliseconds
extern int		curtime;

//============================================================================

//
// NOTE: These functions are forward declared!
//

//
// These functions are implemented inside the "q_sh*" files for platforms
//
// The game DLL must be linked to the same platform as the client for these to work!
//

// TODO: Import these via the import struct?

int		Sys_Milliseconds (void);
void	Sys_Mkdir (char *path);

// pass in an attribute mask of things you wish to REJECT
char*	Sys_FindFirst(char *path, unsigned musthave, unsigned canthave );

char*	Sys_FindNext( unsigned musthave, unsigned canthave );
void	Sys_FindClose(void);

//============================================================================

//
// NOTE: These functions are forward declared!
//

// Their declarations only exist for core functionality!
//
// The client will implement these as-is
//
// The game will implement bodies that forward to the client version (via imported functions)

void 	Sys_Error(char *error, ...);
void 	Com_Printf(char *msg, ...);

//============================================================================

#endif//ZEALOT_QSYS_H
