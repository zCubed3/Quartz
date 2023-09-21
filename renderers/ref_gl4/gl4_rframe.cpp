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
// gl4_rframe.cpp - Implementations of begin and end frame
//

#include "gl4_ref.hpp"

#include <glad/glad.h>

#ifdef USE_IMGUI

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#endif

//
// R_BeginFrame
//
void R_BeginFrame(float stereo_dist)
{
	//
	// Check if the window has changed size
	// TODO: Use the SDL event instead!
	//
	{
		int 	new_width, new_height;

		SDL_GetWindowSize(gl4_state.sdl_window, &new_width, &new_height);

		if (new_width != gl4_state.width || new_height != gl4_state.height)
			R_NewWindow(new_width, new_height, 0);
	}

	//
	// OpenGL frame commands
	//
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gl4_state.width, gl4_state.height);

	//
	// Begin ImGui
	//
#ifdef USE_IMGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(gl4_state.sdl_window);
	ImGui::NewFrame();
#endif
}

//
// R_EndFrame
//
void R_EndFrame(void)
{
	int		err;

	while ((err = glGetError()) != GL_NO_ERROR)
		ri.Con_Printf(PRINT_ALL, "[RefGL4]: OpenGL error '%i'\n", err);

	//
	// Draw our queued characters
	//
	Draw_FlushCharQueue();

	//
	// End ImGui
	//
#ifdef USE_IMGUI
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

	//
	// Swap our buffers (present the image)
	//
	SDL_GL_SwapWindow(gl4_state.sdl_window);
}

//
// R_RenderFrame
//
void R_RenderFrame(refdef_t *fd)
{

}