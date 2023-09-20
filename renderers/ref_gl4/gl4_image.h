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
	int					upload_width, upload_height;	// after power of two and picmip
	int					registration_sequence;		// 0 = free
	struct msurface_s	*texturechain;	// for sort-by-texture world drawing
	int					texnum;						// gl texture binding
	float				sl, tl, sh, th;				// 0,0 - 1,1 unless part of the scrap
	qboolean			scrap;
	qboolean			has_alpha;

	qboolean 			paletted;
} image_t;

#define	TEXNUM_LIGHTMAPS	1024
#define	TEXNUM_SCRAPS		1152
#define	TEXNUM_IMAGES		1153

#define	MAX_GLTEXTURES		1024

void 	LoadPCX(char *filename, byte **pic, byte **palette, int *width, int *height);

#endif //ZEALOT_GL4_IMAGE_H
