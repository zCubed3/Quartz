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
// gl4_rmain.c - The renderer module "main" file
//

#include "gl4_ref.h"

/*
====================================================================

EXTERN BODIES

====================================================================
*/

gl4_state_t gl4_state;

refimport_t ri;

/*
====================================================================

MODULE IMPLEMENTATION

====================================================================
*/

QEXPORT refexport_t GetRefAPI (refimport_t rimp)
{
	refexport_t	re;

	ri = rimp;

	re.api_version = API_VERSION;

	//
	// State
	//
	re.Init 			= R_Init;
	re.Shutdown 		= R_Shutdown;

	re.AppActivate 		= R_AppActivate;

	re.BeginFrame 		= R_BeginFrame;
	re.EndFrame 		= R_EndFrame;
	re.RenderFrame 		= R_RenderFrame;

	//
	// Drawing
	//
	re.DrawGetPicSize 	= Draw_GetPicSize;
	re.DrawPic 			= Draw_Pic;
	re.DrawStretchPic 	= Draw_StretchPic;
	re.DrawChar 		= Draw_Char;
	re.DrawTileClear 	= Draw_TileClear;
	re.DrawFill 		= Draw_Fill;
	re.DrawFadeScreen	= Draw_FadeScreen;

	//
	// Registration
	//
	re.BeginRegistration 	= R_BeginRegistration;
	re.EndRegistration 		= R_EndRegistration;

	re.RegisterModel 		= R_RegisterModel;
	re.RegisterPic			= R_RegisterPic;
	re.RegisterSkin			= R_RegisterSkin;

	//
	// Misc
	//
	re.CinematicSetPalette = R_SetPalette;
	re.SetSky = R_SetSky;

	// Unsure why this is needed?
	Swap_Init();

	return re;
}