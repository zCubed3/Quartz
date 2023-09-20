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
// gl4_rdraw.c - Drawing functions
//

#include "gl4_ref.h"

#include <glad/glad.h>

#include "../../zealcore/zeal_linked_list.hpp"

//
// Draw_GetPicSize
//
void Draw_GetPicSize(int *w, int *h, char *name)
{

}

//
// Draw_Pic
//
void Draw_Pic(int x, int y, char *name)
{

}

//
// Draw_StretchPic
//
void Draw_StretchPic(int x, int y, int w, int h, char *name)
{

}

//
// Draw_Char
//
typedef struct
{
	float x, y;
	char c;
} char_draw_op_t;

const int	width = 640; // TODO: USE THE GL STATE FFS!
const int 	height = 480;
qboolean	char_init = false;
GLuint		char_vao, char_vbo, char_ibo;

zealLinkedList<char_draw_op_t> draw_ops;

//
// Enqueues a draw operation for a character
//
void Draw_Char(int x, int y, int c)
{
	float tex_x, tex_y;
	float org_x, org_y;

	//
	// Early skips
	//
	if (c == ' ')
		return;

	if (y < -8)
		return;

	tex_x = 1.0F / width; // TODO: Replace with actual width and height
	tex_y = 1.0F / height;

	// Remap our passed x and y into screen coords (they were screen cords "back then")

	org_x = x - (width / 2);
	org_y = y - (height / 2);

	org_x *= tex_x * 2;
	org_y *= tex_y * 2;

	char_draw_op_t draw_op;

	draw_op.x = org_x;
	draw_op.y = org_y;
	draw_op.c = c;

	draw_ops.Push(draw_op);
}

typedef float char_info[4];

GLuint 		char_pibo;
char_info 	char_batch[8192];

//
// Draws all characters that have been queued for drawing
// This uses instancing, so it should be much faster!
//
void Draw_CharQueue()
{
	char_draw_op_t 	draw_op;
	size_t 			draw_count;

	// TODO: Every time the window is resized, recalculate this
	if (!char_init)
	{
		const unsigned short TRIANGLES[6] = {
			0, 1, 2,
			3, 2, 1
		};

		const float CHAR_SIZE = 16;

		float tex_x, tex_y;
		float char_x, char_y;

		tex_x = 1.0 / width;
		tex_y = 1.0 / height;

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
		glGenBuffers(1, &char_pibo);


		glBindBuffer(GL_ARRAY_BUFFER, char_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_data), triangle_data, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, nullptr);


		glBindBuffer(GL_ARRAY_BUFFER, char_pibo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(char_batch), char_batch, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, NULL);
		glVertexAttribDivisor(1, 1);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TRIANGLES), TRIANGLES, GL_STATIC_DRAW);

		char_init = true;
	}

	// TODO: Properly handle too large of instances!

	glBindBuffer(GL_ARRAY_BUFFER, char_pibo);

	draw_count = 0;

	while (draw_ops.Pop(draw_op))
	{
		char_batch[draw_count][0] = draw_op.x;
		char_batch[draw_count][1] = draw_op.y;
		char_batch[draw_count][2] = draw_op.c;
		char_batch[draw_count][3] = 0;

		draw_count += 1;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(char_info) * draw_count, char_batch, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	OGL_BindShader(shader_draw_char);

	glBindVertexArray(char_vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, char_ibo);
	glBindBuffer(GL_ARRAY_BUFFER, char_vbo);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr, draw_count);
}

//
// Draw_TileClear
//
void Draw_TileClear(int x, int y, int w, int h, char *name)
{

}

//
// Draw_Fill
//
void Draw_Fill(int x, int y, int w, int h, int c)
{

}

//
// Draw_FadeScreen
//
void Draw_FadeScreen(void)
{

}

//
// Draw_StretchRaw
//
void Draw_StretchRaw(int x, int y, int w, int h, int cols, int rows, byte *data)
{

}