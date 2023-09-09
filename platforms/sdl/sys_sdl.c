/*
Copyright (C) 1997-2001 Id Software, Inc., 2023 zCubed

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
// sys_sdl.c
//

#include <SDL.h>

#include "../../qcommon/qcommon.h"

#include "../../qcommon/qlib.h"

#include <stdio.h>

int			starttime;
int			ActiveApp;
qboolean	Minimized;

unsigned	sys_msg_time;
unsigned	sys_frame_time;

/*
========================================================================

GAME DLL

========================================================================
*/

// TODO: Allow this to be set at runtime instead
static qlib			game_qlib = NULL;
const char*			game_name = "game";
const char*			game_entry = "GetGameAPI";

/*
=================
Sys_UnloadGame
=================
*/
void Sys_UnloadGame (void)
{
	if (!QLib_UnloadLibrary (game_qlib))
		Com_Error (ERR_FATAL, "FreeLibrary failed for game library");

	game_qlib = NULL;
}

/*
=================
Sys_GetGameAPI

Loads the game dll / so
=================
*/
void *Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	cwd[MAX_OSPATH];

	if (game_qlib != NULL)
		Com_Error (ERR_FATAL, "Sys_GetGameAPI without Sys_UnloadingGame! Only one active game can be loaded at once!");

	Com_sprintf (name, sizeof(name), "%s/%s", cwd, game_name);

	game_qlib = QLib_LoadLibrary (name);

	if (game_qlib != NULL)
	{
		Com_DPrintf ("QLib_LoadLibrary (%s)\n", name);
	}
	else
		Com_Error (ERR_FATAL, "Sys_GetGameAPI failed to find game module!");

	GetGameAPI = QLib_GetFuncPtr (game_qlib, game_entry);

	if (!GetGameAPI)
	{
		Sys_UnloadGame ();
		return NULL;
	}

	return GetGameAPI (parms);
}

/*
===============================================================================

SYSTEM IO

===============================================================================
*/

/*
================
Sys_Error
================
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

	exit (1);
}

/*
================
Sys_Quit
================
*/
void Sys_Quit (void)
{
	CL_Shutdown ();
	Qcommon_Shutdown ();

	// TODO: Free dedicated console

	exit (0);
}

/*
================
Sys_ConsoleInput
================
*/
char *Sys_ConsoleInput (void)
{
	// TODO: Read stdin
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

//================================================================

/*
========================================================================

SYS PROCS

========================================================================
*/

/*
================
Sys_GetClipboardData

================
*/
char *Sys_GetClipboardData( void )
{
	return SDL_GetClipboardText();
}

/*
=================
Sys_AppActivate
=================
*/
void Sys_AppActivate (void)
{
	// TODO
	//SDL_ShowWindow(game_window);
}

/*
================
Sys_SendKeyEvents

Send Key_Event calls
================
*/
void Sys_SendKeyEvents (void)
{
	/*
	MSG        msg;

	while (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage (&msg, NULL, 0, 0))
			Sys_Quit ();
		sys_msg_time = msg.time;
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	*/

	// grab frame time
	// TODO: zCubed: Obey id's fixme?
	sys_frame_time = Sys_Milliseconds();	// FIXME: should this be at start?
}

void Sys_Init (void)
{
	// TODO: OS check?
	// TODO: Anything else?

	// TODO: Dedicated server
}

//================================================================

/*
==================
main

==================
*/
int main(int argc, char** argv)
{
	int time, oldtime, newtime;

	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Unlike sys_win, we do no CD checking

	Qcommon_Init (argc, argv);
	oldtime = Sys_Milliseconds ();

	while (1)
	{
		if (Minimized || (dedicated && dedicated->value) )
		{
			SDL_Delay (1000);
		}

		// TODO: zCubed: Understand why id had this? Was it to prevent divide by zero?
		do
		{
			newtime = Sys_Milliseconds ();
			time = newtime - oldtime;
		} while (time < 1);

		Qcommon_Frame (time);

		oldtime = newtime;
	}
}