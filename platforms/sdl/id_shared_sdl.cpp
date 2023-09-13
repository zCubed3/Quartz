/*
Copyright (C) 1997-2001 Id Software, Inc. 2023 zCubed3 (Liam R.)

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

#endif

#include <SDL.h>

/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

#if defined(WIN32)

char		findbase[MAX_OSPATH];
char		findpath[MAX_OSPATH];
HANDLE		findhandle = nullptr;

static qboolean CompareAttributes( unsigned found, unsigned musthave, unsigned canthave )
{
	if ((found & FILE_ATTRIBUTE_READONLY) && (canthave & SFF_RDONLY))
		return false;

	if ((found & FILE_ATTRIBUTE_HIDDEN) && (canthave & SFF_HIDDEN))
		return false;

	if ((found & FILE_ATTRIBUTE_SYSTEM) && (canthave & SFF_SYSTEM))
		return false;

	if ((found & FILE_ATTRIBUTE_DIRECTORY) && (canthave & SFF_SUBDIR))
		return false;

	if ((found & FILE_ATTRIBUTE_ARCHIVE) && (canthave & SFF_ARCH))
		return false;


	if ((musthave & SFF_RDONLY) && !(found & FILE_ATTRIBUTE_READONLY))
		return false;

	if ((musthave & SFF_HIDDEN) && !(found & FILE_ATTRIBUTE_HIDDEN))
		return false;

	if ((musthave & SFF_SYSTEM) && !(found & FILE_ATTRIBUTE_SYSTEM))
		return false;

	if ((musthave & SFF_SUBDIR) && !(found & FILE_ATTRIBUTE_DIRECTORY))
		return false;

	if ((musthave & SFF_ARCH) && !(found & FILE_ATTRIBUTE_ARCHIVE))
		return false;


	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	WIN32_FIND_DATA findinfo;

	if (findhandle)
		Sys_Error("Sys_BeginFind without close");

	findhandle = nullptr;

	COM_FilePath(path, findbase);
	findhandle = FindFirstFileA(path, &findinfo);

	if (findhandle == nullptr)
		return nullptr;

	if (!CompareAttributes(findinfo.dwFileAttributes, musthave, canthave))
		return nullptr;

	Com_sprintf(findpath, sizeof(findpath), "%s/%s", findbase, findinfo.cFileName);
	return findpath;
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	WIN32_FIND_DATA findinfo;

	if (findhandle == nullptr)
		return nullptr;

	if (!FindNextFileA(findhandle, &findinfo))
		return nullptr;

	if (!CompareAttributes(findinfo.dwFileAttributes, musthave, canthave))
		return nullptr;

	Com_sprintf(findpath, sizeof(findpath), "%s/%s", findbase, findinfo.cFileName);
	return findpath;
}

void Sys_FindClose (void)
{
	if (findhandle != nullptr)
	{
		FindClose(findhandle);
		findhandle = nullptr;
	}
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