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
// gl4_rstate.cpp - Handles the state section of the GL4 module
//

#include "gl4_ref.hpp"

//
// R_Init
//
qboolean R_Init(void *param1, void *param2)
{
	// param1 and param2 are unused
	// They're "legacy" things

	// Initialize OpenGL first
	if (!OGL_Init())
		return false;

	// Then initialize the renderer resources
	if (!R_LoadDefaultAssets())
		return false;

	// Set up our initial OpenGL state
	// This can change!
	OGL_DefaultState();

	return true;
}

//
// R_Shutdown
//
void R_Shutdown(void)
{
	OGL_Shutdown();
}

//
// AppActivate
//
void R_AppActivate(qboolean active)
{
	// TODO: AppActivate
}