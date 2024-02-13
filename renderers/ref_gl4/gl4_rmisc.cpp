/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2023-2024 Liam Reese (zCubed3)

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
// gl4_rmisc.cpp - Misc functions
//

#include "gl4_ref.hpp"

//
// R_SetSky
//
void R_SetSky(char *name, float rotate, vec3_t axis)
{

}

//
// R_SetPalette
//
void R_SetPalette(const byte *palette)
{

}

//
// R_NewWindow
//
void R_NewWindow(int width, int height, int fullscreen_mode)
{
	cvar_t*	vid_xpos;
	cvar_t* vid_ypos;
	int		x, y;
	char	title[256];
	void*	extra_data;

	extra_data = nullptr;

	//
	// Update our SDL window
	//
	if (fullscreen_mode == 1)
	{
		x = 0;
		y = 0;
	} else
	{
		vid_xpos = ri.Cvar_Get("vid_xpos", "0", 0);
		vid_ypos = ri.Cvar_Get("vid_ypos", "0", 0);

		x = vid_xpos->value;
		y = vid_ypos->value;
	}

	// If the window doesn't exist, error
	if (!gl4_state.sdl_window)
		ri.Sys_Error(ERR_FATAL, "SDL window was nullptr!!");

	// Update our window
	// (Position only if fullscreen isn't greater than one)
	if (fullscreen_mode == 0)
		SDL_SetWindowPosition(gl4_state.sdl_window, x, y);

	SDL_SetWindowSize(gl4_state.sdl_window, width, height);

	// Set fullscreen
	// TODO: Borderless?
	if (fullscreen_mode == 1)
		SDL_SetWindowFullscreen(gl4_state.sdl_window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(gl4_state.sdl_window, 0);

	sprintf(title, "Zealot (Quake 2) - OpenGL 4.0 + SDL2 (%i x %i)", width, height);

	SDL_SetWindowTitle(gl4_state.sdl_window, title);
	SDL_RaiseWindow(gl4_state.sdl_window);

	//
	// Tell everyone about the window changes
	//

	// TODO: Do these ever need to be dirtied?
	//char_assets_dirty 	= true;
	//pic_assets_dirty 	= true;

	gl4_state.width 	= width;
	gl4_state.height 	= height;

#ifdef USE_IMGUI
	extra_data = gl4_state.imgui_ctx;
#endif

	ri.Vid_NewWindow(gl4_state.sdl_window, extra_data, width, height);
}