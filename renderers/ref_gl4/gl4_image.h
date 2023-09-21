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
// gl4_ref.h - GL4 image asset
//

#ifndef ZEALOT_GL4_IMAGE_H
#define ZEALOT_GL4_IMAGE_H

#include "../../../qcommon/qcommon.h"

typedef enum
{
	IMAGE_TYPE_SKIN,
	IMAGE_TYPE_SPRITE,
	IMAGE_TYPE_WALL,
	IMAGE_TYPE_PIC,
	IMAGE_TYPE_SKY
} imagetype_t;

typedef struct image_s
{
	char				name[MAX_QPATH];			// game path, including extension
	imagetype_t			type;
	int					width, height;				// source image

	struct msurface_s	*texturechain;				// for sort-by-texture world drawing
	unsigned int		handle;						// gl texture binding

	qboolean			has_alpha;
} image_t;

#define	TEXNUM_LIGHTMAPS	1024
#define	TEXNUM_SCRAPS		1152
#define	TEXNUM_IMAGES		1153

#define	MAX_GLTEXTURES		1024

//============================================================================

//
// PCX loading functions
//
typedef struct
{
	byte*	pixels;
	byte*	palette;

	int 	width, height;
} pcx_info_t;

void 	LoadPCX(const char *filename, pcx_info_t* info);
void	ReleasePCX(pcx_info_t* info);

//============================================================================

//
// OpenGL functions
//

void 		OGL_BindImage(image_t* image);

image_t*	OGL_LoadPCX(pcx_info_t* info);

//============================================================================

#endif //ZEALOT_GL4_IMAGE_H
