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
// q_shsdl.cpp
//

#include "../../qcommon/qcommon.h"

#include "cpp/sdl_system.hpp"

#include <SDL.h>

/*
========================================================================

VARIABLE DEFINITIONS

========================================================================
*/

int			curtime;

//===============================================================================



/*
========================================================================

C++ <---> C GLUE

========================================================================
*/

int Sys_Milliseconds (void)
{
	return ZealotQ2::sdl_system.GetMilliseconds();
}

//===============================================================================

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	return ZealotQ2::sdl_system.FindFirst(path, musthave, canthave);
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	return ZealotQ2::sdl_system.FindNext(musthave, canthave);
}

void Sys_FindClose (void)
{
	return ZealotQ2::sdl_system.FindClose();
}

void Sys_Mkdir (char *path)
{
	return ZealotQ2::sdl_system.CreateDir(path);
}

//===============================================================================