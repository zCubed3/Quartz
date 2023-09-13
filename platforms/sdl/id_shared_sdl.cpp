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
// id_shared_sdl.cpp
//

#include "../../qcommon/qcommon.h"

#if defined(WIN32)

#include <Windows.h>
#include <io.h>

#endif

#include <SDL.h>

/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

#if defined(WIN32)

char	findbase[MAX_OSPATH];
char	findpath[MAX_OSPATH];
int		findhandle;

static qboolean CompareAttributes( unsigned found, unsigned musthave, unsigned canthave )
{
	if ( ( found & _A_RDONLY ) && ( canthave & SFF_RDONLY ) )
		return false;
	if ( ( found & _A_HIDDEN ) && ( canthave & SFF_HIDDEN ) )
		return false;
	if ( ( found & _A_SYSTEM ) && ( canthave & SFF_SYSTEM ) )
		return false;
	if ( ( found & _A_SUBDIR ) && ( canthave & SFF_SUBDIR ) )
		return false;
	if ( ( found & _A_ARCH ) && ( canthave & SFF_ARCH ) )
		return false;

	if ( ( musthave & SFF_RDONLY ) && !( found & _A_RDONLY ) )
		return false;
	if ( ( musthave & SFF_HIDDEN ) && !( found & _A_HIDDEN ) )
		return false;
	if ( ( musthave & SFF_SYSTEM ) && !( found & _A_SYSTEM ) )
		return false;
	if ( ( musthave & SFF_SUBDIR ) && !( found & _A_SUBDIR ) )
		return false;
	if ( ( musthave & SFF_ARCH ) && !( found & _A_ARCH ) )
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle)
		Sys_Error ("Sys_BeginFind without close");
	findhandle = 0;

	COM_FilePath (path, findbase);
	findhandle = _findfirst (path, &findinfo);
	if (findhandle == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;
	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle == -1)
		return NULL;
	if (_findnext (findhandle, &findinfo) == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;

	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

void Sys_FindClose (void)
{
	if (findhandle != -1)
		_findclose (findhandle);
	findhandle = 0;
}


void Sys_Mkdir (char *path)
{
	CreateDirectoryA(path, NULL);
}

#endif

//===============================================================================

/*
================
Sys_Milliseconds
================
*/
int	curtime;
int Sys_Milliseconds (void)
{
	curtime = SDL_GetTicks();
	return curtime;
}