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
// sdl_system.cpp
//

#include "sdl_system.hpp"

/*
========================================================================

C++ IMPLEMENTATION

========================================================================
*/
using namespace ZealotQ2;

SDLSystem ZealotQ2::sdl_system;

//================
// State
//================

void SDLSystem::Init()
{
	// TODO: Allocate a console for debug builds / server?
}

void SDLSystem::Quit()
{
	CL_Shutdown();
	Qcommon_Shutdown();

	exit(0);
}

void SDLSystem::Error(const char *error, va_list va)
{
	char		text[1024];

	CL_Shutdown();
	Qcommon_Shutdown();

	vsprintf(text, error, va);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", text, nullptr);

	exit(1);
}

//================
// Getters
//================

void *SDLSystem::GetGameAPI(void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	*path;
	char	cwd[MAX_OSPATH];

	if (qlib_game)
		Com_Error(ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");

	// TODO: Do we need to use CWD?
	Sys_GetCurrentDir(cwd, sizeof(cwd));

	Com_sprintf(name, sizeof(name), "%s/%s%s", cwd, game_name, qlib_postfix);
	qlib_game = QLib_LoadLibrary(name);

	if (qlib_game)
		Com_DPrintf("QLib_LoadLibrary (%s)\n", name);
	else
		Com_DPrintf("Failed to load game library (%s)\n", name);

	GetGameAPI = (qlib_mod_fptr)QLib_GetFuncPtr(qlib_game, "GetGameAPI");
	if (!GetGameAPI)
	{
		Sys_UnloadGame();
		return NULL;
	}

	return GetGameAPI(parms);
}

char *SDLSystem::GetClipboardData()
{
	return SDL_GetClipboardText();
}

// TODO: Replace all integer ms usages with long
int SDLSystem::GetMilliseconds()
{
	curtime = SDL_GetTicks();
	return curtime;
}

//================
// IO
//================

#ifdef WIN32
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

char *SDLSystem::FindFirst(char *path, unsigned musthave, unsigned canthave)
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

char *SDLSystem::FindNext(unsigned musthave, unsigned canthave)
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

void SDLSystem::FindClose()
{
	if (findhandle != -1)
		_findclose (findhandle);
	findhandle = 0;
}

void SDLSystem::CreateDir(const char *path)
{
	CreateDirectoryA(path, nullptr);
}
#endif

//================
// Hunk
//================

void *SDLSystem::HunkAlloc(int size)
{
	// round to cacheline
	size = (size+31)&~31;

	cursize += size;
	if (cursize > hunkmaxsize)
		Sys_Error("Hunk_Alloc overflow");

	return (void *)(membase+cursize-size);
}

void SDLSystem::HunkFree(void *base)
{
	if (base)
		free(base);

	hunkcount--;
}

void *SDLSystem::HunkBegin(int maxsize)
{
	// reserve a huge chunk of memory, but don't commit any yet
	cursize = 0;
	hunkmaxsize = maxsize;

	membase = static_cast<unsigned char*>(malloc(maxsize));
	memset (membase, 0, maxsize);

	if (!membase)
		Sys_Error("VirtualAlloc reserve failed");

	return (void *)membase;
}

int SDLSystem::HunkEnd()
{
	hunkcount++;

	return cursize;
}
