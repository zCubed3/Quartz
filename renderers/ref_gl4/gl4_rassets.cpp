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
// gl4_rassets.cpp - Default asset loading
//

#include "gl4_ref.hpp"

#include "gl4_image.hpp"

#include <glad/glad.h>

gl4_shader_t	*shader_hello_tri;
gl4_shader_t	*shader_draw_char;
gl4_shader_t	*shader_draw_pic;
gl4_shader_t	*shader_unlit_model;

image_t 		*image_conchars;

//
// LoadAllText
//
// Loads all the text from a file
// You're responsible for freeing the memory
//
char *LoadAllText(const char *path)
{
	FILE*	file;
	long 	len;
	char*	text;

	file = fopen(path, "r");

	if (file == nullptr)
		return nullptr;

	fseek(file, 0, SEEK_END);
	len = ftell(file);
	fseek(file, 0, SEEK_SET);

	text = new char[len + 1];

	// Force zero terminate the text (some compilers don't init `new` memory)
	memset(text, 0, len + 1);

	fread(text, len, 1, file);

	fclose(file);
	return text;
}

//
// BuildModule
//
qboolean BuildModule(int shader, char* src)
{
	qboolean	compiled;
	const char*	srcs[1];
	int 		src_len;

	if (src == nullptr)
		return false;

	srcs[0] = src;
	src_len = strlen(src);

	glShaderSource(shader, 1, srcs, &src_len);
	glCompileShader(shader);

	// Did it compile?
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		char		msg[1024];
		int 		msg_len;

		glGetShaderInfoLog(shader, sizeof(msg), &msg_len, msg);

		if (msg_len > 0)
			ri.Con_Printf(PRINT_ALL, "[RefGL4]: Failed to compile shader with error\n\n'%s'\n", msg);
	}

	return compiled;
}

//
// BuildShader
//
// Consumes a shader prototype and returns a built shader
//
gl4_shader_t *BuildShader(gl4_shader_proto_t *proto)
{
	qboolean		success;
	int				shader_vert, shader_frag;
	gl4_shader_t 	*shader;

	if (proto == NULL)
		ri.Sys_Error(ERR_FATAL, "[RefGL4]: Shader prototype was NULL!");

	shader = NULL;
	success = true;

	// Allocate OpenGL handles
	shader_vert = glCreateShader(GL_VERTEX_SHADER);
	shader_frag = glCreateShader(GL_FRAGMENT_SHADER);

	if (!BuildModule(shader_vert, proto->src_vert))
	{
		success = false;
		ri.Con_Printf(PRINT_ALL, "[RefGL4]: Failed to build vertex shader!");
	}


	if (!BuildModule(shader_frag, proto->src_frag))
	{
		success = false;
		ri.Con_Printf(PRINT_ALL, "[RefGL4]: Failed to build fragment shader!");
	}

	if (success)
	{
		qboolean 	compiled;
		int 		program;

		program = glCreateProgram();

		glAttachShader(program, shader_vert);
		glAttachShader(program, shader_frag);

		glLinkProgram(program);

		glDetachShader(program, shader_vert);
		glDetachShader(program, shader_frag);

		glDeleteShader(shader_vert);
		glDeleteShader(shader_frag);

		glGetProgramiv(program, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			char		msg[1024];
			int 		msg_len;

			glGetProgramInfoLog(program, sizeof(msg), &msg_len, msg);

			ri.Con_Printf(PRINT_ALL, "[RefGL4]: Failed to link shader program with error\n\n'%s'\n", msg);

			glDeleteProgram(program);
			success = false;
		}
		else
		{
			shader = new gl4_shader_t;
			shader->handle = program;
		}
	}

	// Clean up the prototype
	delete proto->src_vert;
	delete proto->src_frag;

	proto->src_vert = NULL;
	proto->src_frag = NULL;

	// Return the shader
	return shader;
}

//
// R_LoadDefaultAssets
//
qboolean R_LoadDefaultAssets(void)
{
	//
	// Hello Tri shader
	//
	{
		gl4_shader_proto_t proto;

		proto.src_vert = LoadAllText("ref_gl4/shaders/hello_tri.vert.glsl");
		proto.src_frag = LoadAllText("ref_gl4/shaders/hello_tri.frag.glsl");

		shader_hello_tri = BuildShader(&proto);

		if (shader_hello_tri == nullptr)
			ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to build 'hello tri' shader!");
	}

    //
    // Unlit Model Texture
    //
    {
        gl4_shader_proto_t proto;

        proto.src_vert = LoadAllText("ref_gl4/shaders/unlit_model.vert.glsl");
        proto.src_frag = LoadAllText("ref_gl4/shaders/unlit_model.frag.glsl");

        shader_unlit_model = BuildShader(&proto);

        if (shader_unlit_model == nullptr)
            ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to build 'unlit model' shader!");
    }

	//
	// Draw Char shader
	//
	{
		gl4_shader_proto_t proto;

		proto.src_vert = LoadAllText("ref_gl4/shaders/draw_char.vert.glsl");
		proto.src_frag = LoadAllText("ref_gl4/shaders/draw_char.frag.glsl");

		shader_draw_char = BuildShader(&proto);

		if (shader_draw_char == nullptr)
			ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to build 'draw char' shader!");
	}

	//
	// Draw Pic shader
	//
	{
		gl4_shader_proto_t proto;

		proto.src_vert = LoadAllText("ref_gl4/shaders/draw_pic.vert.glsl");
		proto.src_frag = LoadAllText("ref_gl4/shaders/draw_pic.frag.glsl");

		shader_draw_pic = BuildShader(&proto);

		if (shader_draw_pic == nullptr)
			ri.Sys_Error(ERR_FATAL, "[RefGL4]: Failed to build 'draw pic' shader!");
	}

	//
	// Draw Char Atlas
	//

	// TODO: Make a GL texture type
	{
		pcx_info_t	pcx_info;
		image_t* 	image;

		// If the PCX loaded correctly, we need to convert it to a GL image
		LoadPCX("pics/conchars.pcx", &pcx_info);

		image_conchars = OGL_LoadPCX(&pcx_info);

		ReleasePCX(&pcx_info);
	}

	return true;
}

image_t* Draw_FindPic(const char* name)
{
	image_t*	gl;
	char		fullname[MAX_QPATH];

	if (name[0] != '/' && name[0] != '\\')
	{
		Com_sprintf (fullname, sizeof(fullname), "pics/%s.pcx", name);
		gl = OGL_FindImage(fullname); // TODO: IMAGE_TYPE_PIC
	}
	else
		gl = OGL_FindImage(name+1); // TODO: IMAGE_TYPE_PIC

	return gl;
}