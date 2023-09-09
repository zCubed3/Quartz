/*
Copyright (C) 2023 zCubed

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
// glimp_sdl.c
//

#include "../../../ref_gl/gl_local.h"

void GLimp_BeginFrame( float camera_separation )
{
}

void GLimp_EndFrame( void )
{
}

int GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	return 1;
}

void GLimp_AppActivate( qboolean active )
{
}

void GLimp_EnableLogging( qboolean enable )
{
}

void GLimp_LogNewFrame( void )
{
}

//===============================================================================

/*
================
GLimp_Init
================
*/
qboolean GLimp_Init( void *hinstance, void *hWnd )
{
	// hInstance is actually our SDL window handle
	// We create the GL context here
	SDL_GL_CreateContext(hinstance);

	return 1;
}

/*
================
GLimp_Shutdown
================
*/
void GLimp_Shutdown( void )
{
}