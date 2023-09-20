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
// gl4_ogl.c - OpenGL binding layer, aka "OGL"
//

#include "gl4_ref.h"

#include <glad/glad.h>

#ifdef USE_IMGUI

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#endif

/*
====================================================================

OPENGL STATE MANAGEMENT

====================================================================
*/

//
// Initialization
//
qboolean OGL_Init()
{
	void	*extra; // If USE_IMGUI, then this is the imgui context

	// Ensure SDL is initialized
	SDL_Init(SDL_INIT_VIDEO);

	// Provide GL context hints
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//
	// Create an SDL window
	//
	gl4_state.sdl_window = SDL_CreateWindow(
		"Zealot [OpenGL 4.0]",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);

	ri.Con_Printf(PRINT_ALL, "*** OGL_Init ***\n");

	if (gl4_state.sdl_window == NULL)
		ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to create window with error '%s'", SDL_GetError());
	else
		ri.Con_Printf(PRINT_ALL, "Created SDL window successfully!\n");

	// Set the resolution info
	// TODO: Actually have a new window function that does this!
	gl4_state.width = 640;
	gl4_state.height = 480;

	//
	// Then the GL context
	//
	gl4_state.sdl_gl_ctx = SDL_GL_CreateContext(gl4_state.sdl_window);

	if (gl4_state.sdl_gl_ctx == NULL)
	{
		OGL_CleanupSDL();
		ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to create SDL GL Context with error '%s'", SDL_GetError());
	}
	else
	{
		ri.Con_Printf(PRINT_ALL, "Created GL context successfully!\n");
	}

	//
	// Then load GLAD
	//
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		OGL_CleanupSDL();
		ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to load GLAD!");

		return false;
	}

	//
	// Then initialize ImGui
	//
	extra = nullptr;

#ifdef USE_IMGUI
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(gl4_state.sdl_window, gl4_state.sdl_gl_ctx);
	ImGui_ImplOpenGL3_Init("#version 150");

	gl4_state.imgui_ctx = ImGui::GetCurrentContext();

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);

	extra = gl4_state.imgui_ctx;
#endif

	ri.Vid_NewWindow(gl4_state.sdl_window, extra, 640, 480);
	ri.Con_Printf(PRINT_ALL, "****************\n");

	return true;
}

//
// Shutdown
//
// Calls OGL_CleanupSDL(), ensuring we don't leak any memory on our way out!
//
void OGL_Shutdown()
{
	OGL_CleanupSDL();
}

//
// Cleanup
//
// Destroys the window and gl context (if present)
//
void OGL_CleanupSDL()
{
	// Ensure our window is destroyed
	if (gl4_state.sdl_window != NULL)
	{
		SDL_DestroyWindow(gl4_state.sdl_window);
		gl4_state.sdl_window = NULL;
	}

	// Same with our GL context
	if (gl4_state.sdl_gl_ctx != NULL)
	{
		SDL_GL_DeleteContext(gl4_state.sdl_gl_ctx);
	}
}

//
// OGL_DefaultState
//
// Initializes OpenGL into the default state we use (enabling features such as backface culling)
//
void OGL_DefaultState()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

//
// OGL_BindShader
//
// Binds the provided shader, given that it isn't null
//
void OGL_BindShader(gl4_shader_t *shader)
{
	if (shader == NULL)
		ri.Sys_Error(ERR_FATAL, "[RefGL4]: Shader was NULL!");

	glUseProgram(shader->handle);
}
