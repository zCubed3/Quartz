/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2023-2024 Liam Reese (zCubed3)

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
// gl4_dtext.cpp - Text drawing functions
//

//============================================================================

#include "../gl4_ref.hpp"

#include "../gl4_image.hpp"

#include <glad/glad.h>

#include "../../zealcore/zeal_stack.hpp"

//============================================================================

typedef struct
{
	float x, y;
	char c;
} queued_char_t;

typedef float	gpu_char_t[4];

//============================================================================

// Our queue of characters to draw
zealStack<queued_char_t>	queued_chars = {};

// Our batch size
const int 		CHAR_BATCH_SIZE = 512;

// The actual batch data
gpu_char_t		char_batch[CHAR_BATCH_SIZE];

//============================================================================

//
// OpenGL assets
//
GLuint 		char_vao 			= GL_INVALID_INDEX;
GLuint 		char_vbo 			= GL_INVALID_INDEX;
GLuint 		char_ibo		 	= GL_INVALID_INDEX;
GLuint 		char_instance_vbo 	= GL_INVALID_INDEX;
GLuint		u_charaspect		= GL_INVALID_INDEX;

//============================================================================

// Do character assets need to be initialized?
qboolean	char_assets_dirty = true;

//============================================================================

const float CHAR_SIZE = 16;

//============================================================================

//
// Initializes / updates our character drawing assets
//
void WarmCharAssets()
{
	if (char_assets_dirty)
	{
		const unsigned short TRIANGLES[6] = {
			0, 2, 1,
			3, 1, 2
		};

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

		glGenVertexArrays(1, &char_vao);
		glBindVertexArray(char_vao);

		glGenBuffers(1, &char_vbo);
		glGenBuffers(1, &char_ibo);
		glGenBuffers(1, &char_instance_vbo);

		//
		// Shared VBO
		//
		glBindBuffer(GL_ARRAY_BUFFER, char_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

		//
		// Per instance VBO
		//
		glBindBuffer(GL_ARRAY_BUFFER, char_instance_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(char_batch), char_batch, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
		glVertexAttribDivisor(1, 1);

		//
		// Index Buffer Object
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRIANGLES), TRIANGLES, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

		char_assets_dirty = false;
	}
}

//============================================================================

//
// Enqueues a draw operation for a character
//
void Draw_Char(int x, int y, int c)
{
	queued_char_t 	queued;

	//
	// Early skips
	//
	if (c == ' ')
		return;

	if (y < -8)
		return;

	//
	// Character queuing
	//
	OGL_PixelToClip(x, y, queued.x, queued.y);

	queued.c = c;

	queued_chars.Push(queued);
}

//============================================================================

//
// Flushes the character draw queue (drawing all characters that are queued)
//
void Draw_FlushCharQueue()
{
	queued_char_t 	queued_char;
	size_t 			queue_count;

	//
	// Ensure our character data is warmed up
	//
	WarmCharAssets();

	//
	// Set up the GL state
	//
    // TODO: Not do this
    glDepthFunc(GL_ALWAYS);
    glCullFace(GL_BACK);

	OGL_BindImage(image_conchars);
	OGL_BindShader(shader_draw_char);

    glBindVertexArray(char_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
    glBindBuffer(GL_ARRAY_BUFFER, char_vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

	//
	// Get and update our uniform
	//
	if (u_charaspect == GL_INVALID_INDEX)
		u_charaspect = glGetUniformLocation(shader_draw_char->handle, "u_CharAspect");

	glUniform2f(u_charaspect, 1.0F / gl4_state.width, 1.0F / gl4_state.height);

	//
	// Per batch size we issue a draw call
	//
	queue_count = queued_chars.Count();

	for (size_t b = 0; b <= queue_count; b += CHAR_BATCH_SIZE)
	{
		size_t 		draw_count;

		draw_count = 0;

		glBindBuffer(GL_ARRAY_BUFFER, char_instance_vbo);

		while (draw_count < CHAR_BATCH_SIZE && queued_chars.Pop(queued_char))
		{
			char_batch[draw_count][0] = queued_char.x;
			char_batch[draw_count][1] = queued_char.y;
			char_batch[draw_count][2] = queued_char.c;
			char_batch[draw_count][3] = 0;

			draw_count += 1;
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(gpu_char_t) * draw_count, char_batch, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);
        glVertexAttribDivisor(1, 1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, draw_count);
	}
}

//============================================================================