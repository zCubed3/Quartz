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
// sys_win.h

#include "../../qcommon/qcommon.h"
#include "../../qcommon/qlib.h"

#include "sdlquake.h"

#include <errno.h>
#include <stdio.h>

#include <SDL.h>

// TODO: zCubed: Remove all the ancient Win32 code entirely!

int			ActiveApp;
qboolean	Minimized;

unsigned	sys_msg_time;
unsigned	sys_frame_time;

#define	MAX_NUM_ARGVS	128
int			argc;
char		*argv[MAX_NUM_ARGVS];


/*
===============================================================================

SYSTEM IO

===============================================================================
*/


void Sys_Error (char *error, ...)
{
	va_list		argptr;
	char		text[1024];

	CL_Shutdown ();
	Qcommon_Shutdown ();

	va_start (argptr, error);
	vsprintf (text, error, argptr);
	va_end (argptr);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error!", text, NULL);

	exit(1);
}

void Sys_Quit (void)
{
	timeEndPeriod( 1 );

	CL_Shutdown();
	Qcommon_Shutdown ();

	exit(0);
}

//================================================================

/*
================
Sys_Init
================
*/
void Sys_Init (void)
{
	// TODO: Allocate a console for debug builds / server?
}

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
    MSG        msg;

	while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage (&msg, NULL, 0, 0))
			Sys_Quit ();
		sys_msg_time = msg.time;
      	TranslateMessage (&msg);
      	DispatchMessage (&msg);
	}

	// grab frame time 
	sys_frame_time = timeGetTime();	// FIXME: should this be at start?
}



/*
================
Sys_GetClipboardData

================
*/
char *Sys_GetClipboardData( void )
{
	char *data = NULL;
	char *cliptext;

	if ( OpenClipboard( NULL ) != 0 )
	{
		HANDLE hClipboardData;

		if ( ( hClipboardData = GetClipboardData( CF_TEXT ) ) != 0 )
		{
			if ( ( cliptext = GlobalLock( hClipboardData ) ) != 0 ) 
			{
				data = malloc( GlobalSize( hClipboardData ) + 1 );
				strcpy( data, cliptext );
				GlobalUnlock( hClipboardData );
			}
		}
		CloseClipboard();
	}
	return data;
}

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

static qlib		qlib_game 	= NULL;
const char*		game_name 	= "game"; // TODO: Change this at runtime?

/*
=================
Sys_UnloadGame
=================
*/
void Sys_UnloadGame (void)
{
	if (!QLib_UnloadLibrary(qlib_game))
		Com_Error(ERR_FATAL, "FreeLibrary failed for game library");

	qlib_game = NULL;
}

/*
=================
Sys_GetGameAPI

Loads the game dll
=================
*/
void *Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	*path;
	char	cwd[MAX_OSPATH];

	if (qlib_game)
		Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame");

	// TODO: Do we need to use CWD?
	Sys_GetCurrentDir(cwd, sizeof(cwd));

	Com_sprintf(name, sizeof(name), "%s/%s%s", cwd, game_name, qlib_postfix);
	qlib_game = QLib_LoadLibrary(name);

	if (qlib_game)
		Com_DPrintf("QLib_LoadLibrary (%s)\n", name);
	else
		Com_DPrintf("Failed to load game library (%s)\n", name);

	GetGameAPI = QLib_GetFuncPtr(qlib_game, "GetGameAPI");
	if (!GetGameAPI)
	{
		Sys_UnloadGame();
		return NULL;
	}

	return GetGameAPI(parms);
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

//
