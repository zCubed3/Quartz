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
	//
	// OpenGL frame commands
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, gl4_state.width, gl4_state.height);

	//
	// Calculate our view projection matrices
	//
    glm::mat4x4	projection, view, view_projection;

    glm::mat4x4 model;
    glm::mat4x4 correction_matrix;

    correction_matrix = {
        0, 0, -1, 0,
        -1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 1
    };

    // TODO: Make this a lot simpler!
    view = correction_matrix;
    view *= glm::rotate(glm::radians(-fd->viewangles[2]), glm::vec3(1, 0, 0));
    view *= glm::rotate(glm::radians(-fd->viewangles[0]), glm::vec3(0, 1, 0));
    view *= glm::rotate(glm::radians(-fd->viewangles[1]), glm::vec3(0, 0, 1));
    view = glm::translate(view, glm::vec3(-fd->vieworg[0], -fd->vieworg[1], -fd->vieworg[2]));

    float aspect = (float)fd->width / (float)fd->height;
    projection = glm::perspective(glm::radians(fd->fov_y), aspect, 4.0F, 4096.0F);

    ImGui::Begin("GL4 Debugging");

    //ImGui::Text("Cam Pos: %f, %f, %f", origin.x, origin.y, origin.z);
    //ImGui::Text("Cam Rot: %f, %f, %f", fd->viewangles[0], fd->viewangles[1], fd->viewangles[2]);

    ImGui::Text("%f, %f, %f", fd->vieworg[0], fd->vieworg[1], fd->vieworg[2]);

    ImGui::Spacing();

    ImGui::Text("%f, %f, %f, %f", view[0][0], view[0][1], view[0][2], view[0][3]);
    ImGui::Text("%f, %f, %f, %f", view[1][0], view[1][1], view[1][2], view[1][3]);
    ImGui::Text("%f, %f, %f, %f", view[2][0], view[2][1], view[2][2], view[2][3]);
    ImGui::Text("%f, %f, %f, %f", view[3][0], view[3][1], view[3][2], view[3][3]);

    ImGui::End();

	view_projection = projection * view;

	//
	// Draw the debug triangle
	//
	GLuint	u_viewprojection, u_model;

	{
		OGL_BindShader(shader_unlit_model);

		u_viewprojection = glGetUniformLocation(shader_unlit_model->handle, "u_ViewProjection");
		u_model = glGetUniformLocation(shader_unlit_model->handle, "u_Model");

		glUniformMatrix4fv(u_viewprojection, 1, GL_FALSE, glm::value_ptr(view_projection));
	}

	glBindVertexArray(0);

	//
	// Draw our queue
	//
	for (size_t m = 0; m < fd->num_entities; m++)
	{
		struct entity_s* 	e;
		glm::vec3 			ent_origin;

		e = &fd->entities[m];

        model = glm::translate(glm::identity<glm::mat4x4>(), glm::vec3(e->origin[0], e->origin[1], e->origin[2]));

        model *= glm::rotate(glm::radians(e->angles[1]), glm::vec3(0, 0, 1));
        model *= glm::rotate(glm::radians(e->angles[0]), glm::vec3(0, 1, 0));
        model *= glm::rotate(glm::radians(-e->angles[2]), glm::vec3(1, 0, 0));

		glUniformMatrix4fv(u_model, 1, GL_FALSE, glm::value_ptr(model));

		if (e->model != nullptr)
			OGL_DrawModel(e->model, e->frame, e->oldframe, e->backlerp);

        //ri.Con_Printf(PRINT_ALL, "Drawing Entity at %f, %f, %f\n", entity.origin[0], entity.origin[1], entity.origin[2]);
	}
}

//============================================================================