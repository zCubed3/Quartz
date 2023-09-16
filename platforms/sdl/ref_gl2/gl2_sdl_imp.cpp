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

extern "C" {
	#include "../../../renderers/ref_gl2/gl_local.h"
	#include "../sdlquake.h"
};

#include <cassert>

#include "gl2_sdl.hpp"

#ifdef USE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl2.h>
#endif

extern "C" {
	static qboolean GLimp_SwitchFullscreen(int width, int height);
	qboolean GLimp_InitGL(void);

	glwstate_t glw_state;

	extern cvar_t *vid_fullscreen;
	extern cvar_t *vid_ref;
};

/*
** VID_CreateWindow
*/
qboolean VID_CreateWindow( int width, int height, qboolean fullscreen )
{
	cvar_t 	*vid_xpos, *vid_ypos;
	int		x, y;
	void	*extra;
	char	title[256];

	if (fullscreen == 1)
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
	// (Position only if fullscreen isn't greater than one)
	if (!fullscreen)
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

	sprintf(title, "Zealot (Quake 2) - OpenGL 2.0 + SDL2 (%i x %i)", width, height);

	SDL_SetWindowTitle(glw_state.sdl_window, title);
	SDL_RaiseWindow(glw_state.sdl_window);

	// let the sound and input subsystems know about the new window (and ImGui if initialized)

#if USE_IMGUI
	extra = glw_state.imgui_ctx;
#else
	extra = NULL;
#endif

	ri.Vid_NewWindow(glw_state.sdl_window, extra, width, height);

	return true;
}


/*
** GLimp_SetMode
*/
int GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
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
	// TODO: Init imgui here instead?

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

	// TODO: Make this a helluva lot less ugly and cludgy
	// Has the window changed size?
	// Check if our window size mismatches the vid def
	{
		int 	new_width, new_height;
		SDL_GetWindowSize(glw_state.sdl_window, &new_width, &new_height);

		if (new_width != vid.width || new_height != vid.height)
		{
			vid.width = new_width;
			vid.height = new_height;
			VID_CreateWindow(new_width, new_height, false);
		}
	}

	// TODO: Is there anything we need to do with GLAD here?

	//
	// Begin ImGui
	//
#ifdef USE_IMGUI
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(glw_state.sdl_window);
	ImGui::NewFrame();
#endif
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

	//err = glGetError();
	//assert(err == GL_NO_ERROR);

	//
	// End ImGui
	//
#ifdef USE_IMGUI
	ImGui::Render();

	// Disable Quake's blending
	glDisable(GL_ALPHA_TEST);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	glEnable(GL_ALPHA_TEST);
#endif

	//
	// Present
	//
	SDL_GL_SwapWindow(glw_state.sdl_window);
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
