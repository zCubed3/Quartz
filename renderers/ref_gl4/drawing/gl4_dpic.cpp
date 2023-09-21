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
// gl4_dpic.cpp - Pic drawing functions
//

//============================================================================

#include "../gl4_ref.hpp"

#include "../gl4_image.hpp"

#include <glad/glad.h>

//============================================================================

//
// OpenGL assets
//
GLuint 		pic_vao 	= GL_INVALID_INDEX;
GLuint 		pic_vbo 	= GL_INVALID_INDEX;
GLuint 		pic_ibo 	= GL_INVALID_INDEX;

GLint 		u_picinfo 	= GL_INVALID_INDEX;

//============================================================================

// Do character assets need to be initialized?
qboolean	pic_assets_dirty = true;

//============================================================================

void WarmPicAssets()
{
	// Warm up our assets
	if (pic_assets_dirty)
	{
		const unsigned short TRIANGLES[6] = {
			0, 2, 1,
			3, 1, 2
		};

		// The corner of the screen

		float triangle_data[] = {
			// Vertex 0
			0, 0,
			0, 1,

			// Vertex 1
			1, 0,
			1, 1,

			// Vertex 2
			0, -1,
			0, 0,

			// Vertex 3
			1, -1,
			1, 0
		};

		glGenVertexArrays(1, &pic_vao);
		glBindVertexArray(pic_vao);

		glGenBuffers(1, &pic_vbo);
		glGenBuffers(1, &pic_ibo);

		//
		// Vertex Buffer Object
		//
		glBindBuffer(GL_ARRAY_BUFFER, pic_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

		//
		// Index Buffer Object
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pic_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRIANGLES), TRIANGLES, GL_STATIC_DRAW);

		pic_assets_dirty = false;
	}
}

//============================================================================

//
// Draw_GetPicSize
//
void Draw_GetPicSize(int *w, int *h, char *name)
{
	struct image_s* 	image;

	// Load our image
	image = Draw_FindPic(name);

	if (image != nullptr)
	{
		if (w)
			*w = image->width;

		if (h)
			*h = image->height;
	}
}

//
// Draw_Pic
//
void Draw_Pic(int x, int y, char *name)
{
	struct image_s* 	image;

	float 	tex_x, tex_y;
	float 	org_x, org_y;
	float 	scl_x, scl_y;

	// Warm our drawing assets
	WarmPicAssets();

	// Load our image
	image = Draw_FindPic(name);

	// Bind our required data
	OGL_BindImage(image);
	OGL_BindShader(shader_draw_pic);

	// Ensure we have the uniform
	if (u_picinfo == GL_INVALID_INDEX)
		u_picinfo = glGetUniformLocation(shader_draw_pic->handle, "u_PicInfo");

	// Get the pic location

	// TODO: Put this remapping operation into a function
	// Remap our passed x and y into screen coords (they were screen cords "back then")

	// Orient our image
	tex_x = 1.0 / gl4_state.width;
	tex_y = 1.0 / gl4_state.height;

	OGL_PixelToClip(x, y, org_x, org_y);

	scl_x = image->width * tex_x * 2;
	scl_y = image->height * tex_y * 2;

	// Set the pic info
	glUniform4f(u_picinfo, org_x, -org_y, scl_x, scl_y);

	glBindVertexArray(pic_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pic_ibo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

//
// Draw_StretchPic
//
void Draw_StretchPic(int x, int y, int w, int h, char *name)
{
	struct image_s* 	image;

	float 	tex_x, tex_y;
	float 	org_x, org_y;
	float 	scl_x, scl_y;

	// Warm our drawing assets
	WarmPicAssets();

	// Load our image
	image = Draw_FindPic(name);

	// Bind our required data
	OGL_BindImage(image);
	OGL_BindShader(shader_draw_pic);

	// Ensure we have the uniform
	if (u_picinfo == GL_INVALID_INDEX)
		u_picinfo = glGetUniformLocation(shader_draw_pic->handle, "u_PicInfo");

	// Get the pic location

	// Orient our image
	tex_x = 1.0 / gl4_state.width;
	tex_y = 1.0 / gl4_state.height;

	OGL_PixelToClip(x, y, org_x, org_y);

	scl_x = w * tex_x * 2;
	scl_y = h * tex_y * 2;

	// Set the pic info
	glUniform4f(u_picinfo, org_x, -org_y, scl_x, scl_y);

	glBindVertexArray(pic_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pic_ibo);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

//============================================================================
