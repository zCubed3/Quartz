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
// gl4_dtext.c - Text drawing functions
//

//============================================================================

#include "../gl4_ref.h"

#include <glad/glad.h>

#include "../../zealcore/zeal_linked_list.hpp"

//============================================================================

typedef struct
{
	float x, y;
	char c;
} queued_char_t;

typedef float	gpu_char_t[4];

//============================================================================

// Our queue of characters to draw
zealLinkedList<queued_char_t>	queued_chars = {};

// Our batch size
const int 		CHAR_BATCH_SIZE = 512;

// The actual batch data
gpu_char_t		char_batch[CHAR_BATCH_SIZE];

//============================================================================

//
// OpenGL assets
//
GLuint 		char_vao;
GLuint 		char_vbo;
GLuint 		char_ibo;
GLuint 		char_instance_vbo;

//============================================================================

// Do character assets need to be initialized?
qboolean	char_dirty = true;

//============================================================================

//
// Pushes a new instance vbo (usually done when we go over our batch size)
//
void PushInstanceVBO()
{

}

//
// Initializes / updates our character drawing assets
//
void WarmCharacters()
{
	if (char_dirty)
	{
		const unsigned short TRIANGLES[6] = {
			0, 1, 2,
			3, 2, 1
		};

		const float CHAR_SIZE = 16;

		float tex_x, tex_y;
		float char_x, char_y;

		tex_x = 1.0 / gl4_state.width;
		tex_y = 1.0 / gl4_state.height;

		char_x = tex_x * CHAR_SIZE;
		char_y = tex_y * CHAR_SIZE;

		float triangle_data[] = {
			// Vertex 0
			0, 0,
			0, 0,

			// Vertex 1
			char_x, 0,
			1, 0,

			// Vertex 2
			0, char_y,
			0, 1,

			// Vertex 3
			char_x, char_y,
			1, 1
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
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, NULL);
		glVertexAttribDivisor(1, 1);

		//
		// Index Buffer Object
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRIANGLES), TRIANGLES, GL_STATIC_DRAW);

		char_dirty = false;
	}
}

//============================================================================

//
// Enqueues a draw operation for a character
//
void Draw_Char(int x, int y, int c)
{
	float 			tex_x, tex_y;
	float 			org_x, org_y;
	queued_char_t 	queued;

	//
	// Early skips
	//
	if (c == ' ')
		return;

	if (y < -8)
		return;

	tex_x = 1.0F / gl4_state.width; // TODO: Replace with actual width and height
	tex_y = 1.0F / gl4_state.height;

	// Remap our passed x and y into screen coords (they were screen cords "back then")

	org_x = x - (gl4_state.width / 2);
	org_y = y - (gl4_state.height / 2);

	org_x *= tex_x * 2;
	org_y *= tex_y * 2;

	queued.x = org_x;
	queued.y = org_y;
	queued.c = c;

	queued_chars.Push(queued);
}

//============================================================================

//============================================================================

//
// Flushes the character draw queue (drawing all characters that are queued)
//
void Draw_FlushCharQueue()
{
	queued_char_t 	queued_char;
	size_t 			queue_count;

	// Ensure our character data is warmed up
	WarmCharacters();

	// Set up the GL state
	OGL_BindShader(shader_draw_char);

	glBindVertexArray(char_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
	//glBindBuffer(GL_ARRAY_BUFFER, char_vbo);

	// Per batch size we issue a draw call
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
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, draw_count);
	}
}

//============================================================================