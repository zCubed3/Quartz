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
// gl4_rframe.c - Implementations of begin and end frame
//

#include "gl4_ref.h"

#include <glad/glad.h>

//
// R_BeginFrame
//
void R_BeginFrame(float stereo_dist)
{
	// TODO: Handle window resizing
	// TODO: Handle vsync changes
	int indices[3] = {
		0, 1, 2
	};

	glClear(GL_DEPTH_BUFFER_BIT);

	OGL_BindShader(gl4_shader_hello_tri);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &indices);
}

//
// R_EndFrame
//
void R_EndFrame(void)
{
	int		err;

	while ((err = glGetError()) != GL_NO_ERROR)
		ri.Con_Printf(PRINT_ALL, "[RefGL4]: OpenGL error '%i'\n", err);

	// Swap our buffers (present the image)
	SDL_GL_SwapWindow(gl4_state.sdl_window);
}

//
// R_RenderFrame
//
void R_RenderFrame(refdef_t *fd)
{

}