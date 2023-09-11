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
// sys_sdl.cpp
//

#include "cpp/sdl_system.hpp"

#include "../../qcommon/qcommon.h"
#include "../../qcommon/qlib.h"

#include "sdlquake.h"

#include <cerrno>
#include <cstdio>

#include <SDL.h>

#if defined(WIN32)

#include <io.h>

#endif

// TODO: zCubed: Remove all the ancient Win32 code entirely!

/*
========================================================================

VARIABLE DEFINITIONS

========================================================================
*/

qboolean	ActiveApp;
qboolean	Minimized;

unsigned	sys_msg_time;
unsigned	sys_frame_time;

//================================================================

#define	MAX_NUM_ARGVS	128

int			argc;
char		*argv[MAX_NUM_ARGVS];

/*
========================================================================

C++ <---> C GLUE

========================================================================
*/

void Sys_Init (void)
{
	ZealotQ2::sdl_system.Init();
}

void Sys_Error (char *error, ...)
{
	va_list 	args;

	va_start(args, error);
	ZealotQ2::sdl_system.Error(error, args);
	va_end(args);
}

void Sys_Quit (void)
{
	ZealotQ2::sdl_system.Quit();
}

//================================================================

void* Sys_GetGameAPI(void* parms)
{
	return ZealotQ2::sdl_system.GetGameAPI(parms);
}

// Sys_Milliseconds is in q_shsdl.cpp!

char *Sys_GetClipboardData( void )
{
	return ZealotQ2::sdl_system.GetClipboardData();
}

//================================================================

/*
================
Sys_ConsoleInput
================
*/
char *Sys_ConsoleInput (void)
{
	// TODO: Grab input from stdin?
	// Or... make a dedicated server?

	return NULL;
}


/*
================
Sys_ConsoleOutput

Print text to the dedicated console
================
*/
void Sys_ConsoleOutput (char *string)
{
	puts(string);
}


/*
================
Sys_SendKeyEvents

Send Key_Event calls
================
*/
void Sys_SendKeyEvents (void)
{
	// TODO: Remove this?
}



/*
================
Sys_GetClipboardData

================
*/

/*
==============================================================================

 WINDOWS CRAP

==============================================================================
*/

#if defined(WIN32)
void Sys_GetCurrentDir (char *string, long size)
{
	GetCurrentDirectoryA(size, string);
}
#endif

/*
=================
Sys_AppActivate
=================
*/
void Sys_AppActivate (void)
{
	SDL_RaiseWindow(cl_window);
}

/*
========================================================================

GAME DLL

========================================================================
*/
/*
=================
Sys_UnloadGame
=================
*/


//=======================================================================

/*
==================
main

==================
*/
HINSTANCE	global_hInstance;

int main(int argc, char** argv)
{
    MSG				msg;
	int				time, oldtime, newtime;

	// TODO: This is temporary, replace Win32 entirely at some point!
	global_hInstance = GetModuleHandle(NULL);

	// Initialize SDL!
	SDL_Init(SDL_INIT_EVERYTHING);

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();

    /* main window message loop */
	while (1)
	{
		// Poll SDL events
		IN_PollSDL();

		// if at a full screen console, don't update unless needed
		if (Minimized || (dedicated && dedicated->value))
		{
			Sleep (1);
		}

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
