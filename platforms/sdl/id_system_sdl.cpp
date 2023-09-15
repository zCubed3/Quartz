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
// id_system_sdl.cpp
//

#include "id_system_sdl.hpp"

#include "id_input_sdl.hpp"

#include "../../client/id_client.hpp"

extern "C" {
	#include "../../qcommon/qcommon.h"

	#include "sdlquake.h"
};

#include <cerrno>
#include <cstdio>

#include <SDL.h>

// TODO: zCubed: Remove all the ancient Win32 code entirely!

qboolean	ActiveApp;
qboolean	Minimized;

unsigned	sys_msg_time;
unsigned	sys_frame_time;

int			argc;
char		*argv[MAX_NUM_ARGVS];


/*
===============================================================================

 idSystemSDL IMPLEMENTATION

===============================================================================
*/

idSystem *id_sys 	= new idSystemSDL();

// ==============
//  System State
// ==============
void idSystemSDL::Init()
{

}

void idSystemSDL::Quit()
{
	id_cl->Shutdown();
	Qcommon_Shutdown();

	exit(0);
}

void idSystemSDL::AppActivate()
{
	SDL_RaiseWindow(cl_window);
}

void idSystemSDL::Error(char *error, va_list argptr)
{
	char text[1024];

	id_cl->Shutdown();
	Qcommon_Shutdown();

	vsprintf(text, error, argptr);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", text, nullptr);

	exit(1);
}

// =================
//  Game Management
// =================
void idSystemSDL::UnloadGame()
{
	if (!QLib_UnloadLibrary(qlib_game))
		Com_Error(ERR_FATAL, "QLib_UnloadLibrary failed for game library");

	qlib_game = nullptr;
}

void* idSystemSDL::GetGameAPI(void *parms)
{
	void 	*(*GetGameAPI)(void *);
	char 	*path;
	char 	name[MAX_OSPATH];
	char 	cwd[MAX_OSPATH];
	cvar_t	*sys_game_lib;

	if (qlib_game)
		Com_Error(ERR_FATAL, "idSystem::GetGameAPI without idSystem::UnloadGame");

	// TODO: Do we need to use CWD?
	Sys_GetCurrentDir(cwd, sizeof(cwd));

	// TODO: LOAD GAME NAME AT RUNTIME!

	sys_game_lib = Cvar_Get("sys_game_lib", "game", CVAR_ARCHIVE);

	Com_sprintf(name, sizeof(name), "%s/%s%s", cwd, sys_game_lib->string, qlib_postfix);
	qlib_game = QLib_LoadLibrary(name);

	if (qlib_game)
		Com_DPrintf("QLib_LoadLibrary (%s)\n", name);
	else
		Com_DPrintf("Failed to load game library (%s)\n", name);

	GetGameAPI = QLib_GetFuncPtr(qlib_game, "GetGameAPI");
	if (!GetGameAPI)
	{
		UnloadGame();
		return nullptr;
	}

	return GetGameAPI(parms);
}

// ============
//  Console IO
// ============
char* idSystemSDL::ConsoleInput()
{
	return nullptr;
}

void idSystemSDL::ConsoleOutput(char *string)
{
	puts(string);
}

void idSystemSDL::SendKeyEvents()
{
	// TODO: Why is this still here?
	sys_frame_time = timeGetTime();
}

// ===========
//  System IO
// ===========
char *idSystemSDL::GetClipboardData()
{
	return SDL_GetClipboardText();
}

void idSystemSDL::GetCurrentDir(char *string, long size)
{
#if WIN32
	GetCurrentDirectoryA(size, string);
#endif
}

//================================================================

/*
===============================================================================

 idSystemSDL <-> legacy C bindings

===============================================================================
*/

// ==============
//  System State
// ==============
void Sys_Init(void)
{
	id_sys->Init();
}

void Sys_Quit(void)
{
	id_sys->Quit();
}

void Sys_AppActivate(void)
{
	id_sys->AppActivate();
}

void Sys_Error(char *error, ...)
{
	va_list 	argptr;

	va_start(argptr, error);
	id_sys->Error(error, argptr);
	va_end(argptr);
}

// =================
//  Game Management
// =================
void Sys_UnloadGame(void)
{
	id_sys->UnloadGame();
}

void* Sys_GetGameAPI(void *parms)
{
	return id_sys->GetGameAPI(parms);
}

// ============
//  Console IO
// ============
char* Sys_ConsoleInput(void)
{
	return id_sys->ConsoleInput();
}

void Sys_ConsoleOutput(char *string)
{
	id_sys->ConsoleOutput(string);
}

void Sys_SendKeyEvents(void)
{
	id_sys->SendKeyEvents();
}

// ===========
//  System IO
// ===========
char *Sys_GetClipboardData(void)
{
	return id_sys->GetClipboardData();
}

void Sys_GetCurrentDir(char *string, long size)
{
	return id_sys->GetCurrentDir(string, size);
}

//=======================================================================

/*
==================
main

==================
*/
HINSTANCE	global_hInstance;

int main(int argc, char** argv)
{
	int		time, oldtime, newtime;

	SDL_Init(SDL_INIT_EVERYTHING);

	// TODO: This is temporary, replace Win32 entirely at some point!
	global_hInstance = GetModuleHandle(NULL);

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();

    /* main window message loop */
	while (1)
	{
		// Poll SDL events
		reinterpret_cast<idInputSDL*>(id_in)->PollSDL();

		// if at a full screen console, don't update unless needed
		if (Minimized || (dedicated && dedicated->value))
			SDL_Delay(10000);

		do
		{
			newtime = Sys_Milliseconds();
			time = newtime - oldtime;
		} while (time < 1);

		Qcommon_Frame (time);

		oldtime = newtime;
	}

	// never gets here
    return TRUE;
}

//
