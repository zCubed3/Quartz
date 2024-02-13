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
// gl4_aimage.cpp - Image asset functions
//

//============================================================================

#include "../gl4_ref.hpp"

#include "../gl4_model.hpp"

#include <glad/glad.h>

#include <vector>

#include <glm/glm.hpp>

//============================================================================

// Precalculated normals
float	r_avertexnormals[NUMVERTEXNORMALS][3] = {
#include "../anorms.hpp"
};

//============================================================================

// Our default vertex array
GLuint  gl_SharedVertexArray = GL_INVALID_INDEX;

//============================================================================

// TODO: Move this out of here
typedef struct glvertex_s
{
	glm::vec3 	position;
	glm::vec3	normal;
    glm::vec2 	uv;
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
	// Load the vertices
	// Also convert our disk triangles to real triangles
	//

    // FIXME: Is this anything like id's actual model loading code?
    // FIXME: Move this to a idModel class!

    unsigned short* indices;
    glvertex_t*     vertices;

    indices = new unsigned short[header.num_tris * 3];
    vertices = new glvertex_t[(header.num_frames + 1) * header.num_xyz];

    // FIXME: Actually load frames
    for (int f = 0; f < header.num_frames; f++) {
        daliasframe_t* frame = (daliasframe_t*)(buffer + header.ofs_frames + (header.framesize * f));

        size_t offset = f * (header.num_xyz);
        //ri.Con_Printf(PRINT_ALL, "FRAME %i = %.16s\n", f, frame->name);

        for (int v = 0; v < header.num_xyz; v++) {
            // We need to decompress the vertices

            vertices[v + offset].position[0] = frame->verts[v].v[0];
            vertices[v + offset].position[1] = frame->verts[v].v[1];
            vertices[v + offset].position[2] = frame->verts[v].v[2];

            vertices[v + offset].position[0] *= frame->scale[0];
            vertices[v + offset].position[1] *= frame->scale[1];
            vertices[v + offset].position[2] *= frame->scale[2];

            vertices[v + offset].position[0] += frame->translate[0];
            vertices[v + offset].position[1] += frame->translate[1];
            vertices[v + offset].position[2] += frame->translate[2];

            vertices[v + offset].normal[0] = r_avertexnormals[frame->verts[v].lightnormalindex][0];
            vertices[v + offset].normal[1] = r_avertexnormals[frame->verts[v].lightnormalindex][1];
            vertices[v + offset].normal[2] = r_avertexnormals[frame->verts[v].lightnormalindex][2];
        }
    }

    for (int t = 0; t < header.num_tris; t++) {
        dtriangle_t* tri = (dtriangle_t*)(buffer + header.ofs_tris + (t * sizeof(dtriangle_t)));

        size_t actual = t * 3;

        indices[actual] = tri->index_xyz[0];
        indices[actual + 1] = tri->index_xyz[1];
        indices[actual + 2] = tri->index_xyz[2];
    }

    mod->index_count = header.num_tris * 3;
    mod->vert_count = header.num_xyz;
    mod->vertex_data = vertices;
    mod->vertex_scratch = vertices + (header.num_frames * header.num_xyz);

    size_t data_size = sizeof(glvertex_t) * header.num_xyz;
    size_t index_size = sizeof(unsigned short) * mod->index_count;

    glGenBuffers(1, &mod->gl_vbo);
    glGenBuffers(1, &mod->gl_ibo);

    glGenVertexArrays(1, &mod->gl_vao);
    glBindVertexArray(mod->gl_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mod->gl_vbo);
    glBufferData(GL_ARRAY_BUFFER, data_size, vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glvertex_t), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glvertex_t), (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glvertex_t), (void*)(sizeof(float) * 6));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod->gl_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, indices, GL_STATIC_DRAW);

	mod->type = MODEL_TYPE_ALIAS;

	mod->mins[0] = -32;
	mod->mins[1] = -32;
	mod->mins[2] = -32;
	mod->maxs[0] = 32;
	mod->maxs[1] = 32;
	mod->maxs[2] = 32;
}

//============================================================================

void OGL_DrawModel(struct model_s* model, int frame, int oldframe, float backlerp)
{
    if (model == nullptr) {
        ri.Sys_Error(ERR_FATAL, "Attempted to draw a model which is a nullptr!");
        return;
    }

    //
    // Vertex animation
    //
    float frontlerp = 1.0F - backlerp;

    size_t start = frame * model->vert_count;
    size_t end = oldframe * model->vert_count;

    size_t data_size = sizeof(glvertex_t) * model->vert_count;

    for (int v = 0; v < model->vert_count; v++) {
        model->vertex_scratch[v].position
            = glm::mix(model->vertex_data[start + v].position, model->vertex_data[end + v].position, backlerp);

        model->vertex_scratch[v].normal
            = glm::mix(model->vertex_data[start + v].normal, model->vertex_data[end + v].normal, backlerp);
    }

    glBindBuffer(GL_ARRAY_BUFFER, model->gl_vbo);
    glBufferData(GL_ARRAY_BUFFER, data_size, model->vertex_scratch, GL_DYNAMIC_DRAW);

    glBindVertexArray(model->gl_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->gl_ibo);
    glDrawElements(GL_TRIANGLES, model->index_count, GL_UNSIGNED_SHORT, nullptr);
}

//============================================================================
