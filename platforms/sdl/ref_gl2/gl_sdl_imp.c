/*
Copyright (C) 1997-2001 Id Software, Inc.

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
/*
** GLW_IMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_EndFrame
** GLimp_Init
** GLimp_Shutdown
** GLimp_SwitchFullscreen
**
*/
#include <assert.h>

#include "../../../renderers/ref_gl2/gl_local.h"

#include "../sdlquake.h"

#include "gl_sdl.h"

static qboolean GLimp_SwitchFullscreen( int width, int height );
qboolean GLimp_InitGL (void);

glwstate_t glw_state;

extern cvar_t *vid_fullscreen;
extern cvar_t *vid_ref;

/*
** VID_CreateWindow
*/
qboolean VID_CreateWindow( int width, int height, qboolean fullscreen )
{
	cvar_t *vid_xpos, *vid_ypos;
	int x, y;

	if (fullscreen)
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
	if (!glw_state.sdl_window)
		ri.Sys_Error(ERR_FATAL, "SDL window was NULL!");

	// Update our window
	SDL_SetWindowPosition(glw_state.sdl_window, x, y);
	SDL_SetWindowSize(glw_state.sdl_window, width, height);

	// Set fullscreen
	// TODO: Borderless?
	if (fullscreen)
		SDL_SetWindowFullscreen(glw_state.sdl_window, SDL_WINDOW_FULLSCREEN);
	else
		SDL_SetWindowFullscreen(glw_state.sdl_window, 0);

	// init all the gl stuff for the window
	if (!GLimp_InitGL())
	{
		ri.Con_Printf(PRINT_ALL, "VID_CreateWindow() -> GLimp_InitGL failed\n");
		return false;
	}

	SDL_RaiseWindow(glw_state.sdl_window);

	// let the sound and input subsystems know about the new window
	ri.Vid_NewWindow(glw_state.sdl_window, width, height);

	return true;
}


/*
** GLimp_SetMode
*/
rserr_t GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	int width, height;
	const char *win_fs[] = { "W", "FS" };

	ri.Con_Printf( PRINT_ALL, "Initializing OpenGL display\n");

	ri.Con_Printf (PRINT_ALL, "...setting mode %d:", mode );

	if ( !ri.Vid_GetModeInfo( &width, &height, mode ) )
	{
		ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
		return rserr_invalid_mode;
	}

	ri.Con_Printf( PRINT_ALL, " %d %d %s\n", width, height, win_fs[fullscreen] );

	// destroy the existing window
	//if (glw_state.sdl_window)
	//{
	//	GLimp_Shutdown ();
	//}

	// do a CDS if needed
	*pwidth = width;
	*pheight = height;

	if ( fullscreen )
	{
		ri.Con_Printf( PRINT_ALL, "...attempting fullscreen\n" );

		gl_state.fullscreen = true;

		ri.Con_Printf( PRINT_ALL, "ok\n" );

		if ( !VID_CreateWindow (width, height, true) )
			return rserr_invalid_mode;

		return rserr_ok;
	}
	else
	{
		ri.Con_Printf(PRINT_ALL, "...setting windowed mode\n");

		gl_state.fullscreen = false;

		if (!VID_CreateWindow(width, height, false))
			return rserr_invalid_mode;
	}

	return rserr_ok;
}

/*
** GLimp_Shutdown
**
** This routine does all OS specific shutdown procedures for the OpenGL
** subsystem.  Under OpenGL this means NULLing out the current DC and
** HGLRC, deleting the rendering context, and releasing the DC acquired
** for the window.  The state structure is also nulled out.
**
*/
void GLimp_Shutdown( void )
{
	// TODO: Reset context?
	//SDL_GL_MakeCurrent()

	if (glw_state.sdl_gl_ctx)
	{
		SDL_GL_DeleteContext(glw_state.sdl_gl_ctx);
		//ri.Con_Printf( PRINT_ALL, "[ref_gl::R_Shutdown()] - SDL_Delete_GLContext failed\n");

		glw_state.sdl_gl_ctx = NULL;
	}

	if (glw_state.sdl_window)
	{
		SDL_DestroyWindow(glw_state.sdl_window);

		glw_state.sdl_window = NULL;
	}

	if ( glw_state.log_fp )
	{
		fclose( glw_state.log_fp );
		glw_state.log_fp = 0;
	}

	if ( gl_state.fullscreen )
	{
		gl_state.fullscreen = false;
	}
}


/*
** GLimp_Init
**
** This routine is responsible for initializing the OS specific portions
** of OpenGL.  Under Win32 this means dealing with the pixelformats and
** doing the wgl interface stuff.
*/
qboolean GLimp_Init( void *hinstance, void *wndproc )
{
	/*
	#define OSR2_BUILD_NUMBER 1111

	OSVERSIONINFO	vinfo;

	vinfo.dwOSVersionInfoSize = sizeof(vinfo);

	glw_state.allowdisplaydepthchange = false;

	if ( GetVersionEx( &vinfo) )
	{
		if ( vinfo.dwMajorVersion > 4 )
		{
			glw_state.allowdisplaydepthchange = true;
		}
		else if ( vinfo.dwMajorVersion == 4 )
		{
			if ( vinfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
			{
				glw_state.allowdisplaydepthchange = true;
			}
			else if ( vinfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
			{
				if ( LOWORD( vinfo.dwBuildNumber ) >= OSR2_BUILD_NUMBER )
				{
					glw_state.allowdisplaydepthchange = true;
				}
			}
		}
	}
	else
	{
		ri.Con_Printf( PRINT_ALL, "GLimp_Init() - GetVersionEx failed\n" );
		return false;
	}

	glw_state.hInstance = ( HINSTANCE ) hinstance;
	glw_state.wndproc = wndproc;
	*/

	return true;
}

qboolean GLimp_InitGL (void)
{
	//
	// This is stubbed because Windows and Linux have different init schemes for OpenGL
	// Therefore it all happens EXACTLY when the DLL is loaded
	//

	return true;
}

/*
** GLimp_BeginFrame
*/
void GLimp_BeginFrame( float camera_separation )
{
	/*
	if ( gl_bitdepth->modified )
	{
		if ( gl_bitdepth->value != 0 && !glw_state.allowdisplaydepthchange )
		{
			ri.Cvar_SetValue( "gl_bitdepth", 0 );
			ri.Con_Printf( PRINT_ALL, "gl_bitdepth requires Win95 OSR2.x or WinNT 4.x\n" );
		}
		gl_bitdepth->modified = false;
	}

	if ( camera_separation < 0 && gl_state.stereo_enabled )
	{
		glDrawBuffer( GL_BACK_LEFT );
	}
	else if ( camera_separation > 0 && gl_state.stereo_enabled )
	{
		glDrawBuffer( GL_BACK_RIGHT );
	}
	else
	{
		glDrawBuffer( GL_BACK );
	}
	*/

	// TODO: Is there anything we need to do with GLAD here?
}

/*
** GLimp_EndFrame
** 
** Responsible for doing a swapbuffers and possibly for other stuff
** as yet to be determined.  Probably better not to make this a GLimp
** function and instead do a call to GLimp_SwapBuffers.
*/
void GLimp_EndFrame (void)
{
	int		err;

	err = glGetError();
	assert(err == GL_NO_ERROR);

	if ( stricmp( gl_drawbuffer->string, "GL_BACK" ) == 0 )
	{
		SDL_GL_SwapWindow(glw_state.sdl_window);
		//ri.Sys_Error( ERR_FATAL, "GLimp_EndFrame() - SwapBuffers() failed!\n" );
	}
}

/*
** GLimp_AppActivate
*/
void GLimp_AppActivate( qboolean active )
{
	if ( active )
	{
		SDL_RaiseWindow(glw_state.sdl_window);
	}
	else
	{
		if ( vid_fullscreen->value )
			SDL_MinimizeWindow(glw_state.sdl_window);
	}
}
