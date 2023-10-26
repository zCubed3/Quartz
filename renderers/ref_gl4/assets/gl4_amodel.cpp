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
// gl4_aimage.cpp - Image asset functions
//

//============================================================================

#include "../gl4_ref.hpp"

#include "../gl4_model.hpp"

#include <glad/glad.h>

#include <vector>

//============================================================================

// Our default vertex array
GLuint  gl_SharedVertexArray;

//============================================================================

// TODO: Move this out of here
typedef struct
{
	float 	position[3];
	float 	uv[2];
	float	normal[3];
} glvertex_t;

void Mod_LoadAliasModel (model_t *mod, char *buffer)
{
	int					version;
	dmdl_t				header;

	version = LittleLong(((int*)buffer)[1]);

	if (version != ALIAS_VERSION)
		ri.Sys_Error (ERR_DROP, "%s has wrong version number (%i should be %i)",
					  mod->name, version, ALIAS_VERSION);

	// Copy the first bytes of buffer into the header
	memcpy(&header, buffer, sizeof(dmdl_t));

	// byte swap the header fields and sanity check
	// TODO?
	//for (i = 0; i < sizeof(dmdl_t) / sizeof(int); i++)
	//	((int*)(pheader))[i] = LittleLong(((int*)buffer)[i]);

	if (header.skinheight > MAX_LBM_HEIGHT)
		ri.Sys_Error (ERR_DROP, "model %s has a skin taller than %d", mod->name,
					  MAX_LBM_HEIGHT);

	if (header.num_xyz <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no vertices", mod->name);

	if (header.num_xyz > MAX_VERTS)
		ri.Sys_Error (ERR_DROP, "model %s has too many vertices", mod->name);

	if (header.num_st <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no st vertices", mod->name);

	if (header.num_tris <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no triangles", mod->name);

	if (header.num_frames <= 0)
		ri.Sys_Error (ERR_DROP, "model %s has no frames", mod->name);

	//
	// Load our disk data
	//
	dtriangle_t* 	triangles;
    daliasframe_t*	frames;
	glvertex_t*		gl_vertices;


	triangles = (dtriangle_t*)(buffer + header.ofs_tris);
	frames = (daliasframe_t*)(buffer + header.ofs_frames);

    gl_vertices = new glvertex_t[header.num_xyz];

    size_t seek = 0;
    for (int f = 0; f < header.num_frames; f++) {

    }

	//
	// Load the vertices
	// Also convert our disk triangles to real triangles
	//

    // FIXME

	mod->type = MODEL_TYPE_ALIAS;

	mod->mins[0] = -32;
	mod->mins[1] = -32;
	mod->mins[2] = -32;
	mod->maxs[0] = 32;
	mod->maxs[1] = 32;
	mod->maxs[2] = 32;
}

//============================================================================

void OGL_DrawModel(struct model_s* model)
{
    //glBindVertexArray(gl_SharedVertexArray);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER)
}

//============================================================================
