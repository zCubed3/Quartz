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

//============================================================================

#include "gl4_ref.hpp"

#include <glad/glad.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <glm/gtc/type_ptr.hpp>

//============================================================================

#ifdef USE_IMGUI

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

#endif

//============================================================================

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
	// Begin ImGui
	//
#ifdef USE_IMGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(gl4_state.sdl_window);
	ImGui::NewFrame();
#endif
}

//============================================================================

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

//============================================================================

//
// R_RenderFrame
//
void R_RenderFrame(refdef_t *fd)
{
	glm::mat4x4	projection, view, view_projection;
	glm::quat quaternion, correction;
	glm::vec3 origin, forward;

	glm::mat4x4 model;

	//
	// OpenGL frame commands
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gl4_state.width, gl4_state.height);

	//
	// Calculate our view projection matrices
	//

	// TODO: Calculate aspect
	// TODO: Cvar near and far
	projection = glm::perspective(glm::radians(fd->fov_y), 1.0F, 0.01F, 1000.0F);

	origin = glm::vec3(fd->vieworg[0], fd->vieworg[1], fd->vieworg[2]);
	forward = glm::vec3(0, 0, 1);

	correction = glm::quat(glm::vec3(
		glm::radians(-90.0F),
		glm::radians(0.0F),
		glm::radians(90.0F)
	));

	quaternion = glm::quat(glm::vec3(
		glm::radians(-fd->viewangles[2]),
		glm::radians(-fd->viewangles[0]),
		glm::radians(-fd->viewangles[1])
	));

	forward = correction * quaternion * glm::vec4(forward, 1);
	forward = glm::normalize(forward);

	view = glm::lookAt(origin, origin + forward, glm::vec3(0, 1, 0));

	view_projection = projection * view;

	//
	// Draw the debug triangle
	//
	GLuint	u_viewprojection, u_model;

	{
		OGL_BindShader(shader_hello_tri);

		u_viewprojection = glGetUniformLocation(shader_hello_tri->handle, "u_ViewProjection");
		u_model = glGetUniformLocation(shader_hello_tri->handle, "u_Model");

		glUniformMatrix4fv(u_viewprojection, 1, GL_FALSE, glm::value_ptr(view_projection));
	}

	glBindVertexArray(0);

	//
	// Draw our queue
	//
	for (size_t m = 0; m < fd->num_entities; m++)
	{
		struct entity_s 	entity;
		glm::vec3 			ent_origin;

		entity = fd->entities[m];

		ent_origin = glm::vec3(entity.origin[0], entity.origin[1], entity.origin[2]);

		model = glm::translate(glm::mat4x4(1.0F), ent_origin);

		glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));

		if (!fd->entities[m].model)
			OGL_DrawModel(fd->entities[m].model);
	}
}

//============================================================================