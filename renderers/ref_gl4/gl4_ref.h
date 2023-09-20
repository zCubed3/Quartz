/*
Copyright (C) 1997-2001 Id Software, Inc.2023 zCubed3 (Liam R.)

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
// gl4_ref.h - GL4 renderer common header
//

#ifndef ZEALOT_GL4_REF_H
#define ZEALOT_GL4_REF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../client/ref.h"

#ifdef __cplusplus
};
#endif

#include <SDL.h>

#if defined(USE_IMGUI) && defined(__cplusplus)
#include <imgui.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
====================================================================

SHADER STRUCTURES

====================================================================
*/

typedef struct
{
	char 	*src_vert;
	char	*src_frag;
} gl4_shader_proto_t;

typedef struct
{
	// We don't store sources, use gl4_shader_proto instead!
	int		handle;
} gl4_shader_t;

/*
====================================================================

IMAGE STRUCTURES

====================================================================
*/
typedef struct
{
	// We don't store the image info, you need a gl4_image_proto for that!
	int		handle;
} gl4_image_t;


/*
====================================================================

STRUCTURES

====================================================================
*/
typedef struct
{
	// SDL data
	SDL_Window*		sdl_window;
	SDL_GLContext	sdl_gl_ctx;

	// Window data
	int 			width;
	int 			height;

#ifdef USE_IMGUI
	void*			imgui_ctx;
#endif
} gl4_state_t;

extern 	gl4_state_t 	gl4_state;

/*
====================================================================

MODULE FUNCTIONS

====================================================================
*/

// ==============
//  Render State
// ==============
qboolean 	R_Init(void *param1, void *param2);
void		R_Shutdown(void);

void		R_BeginFrame(float stereo_dist);
void		R_EndFrame(void);
void		R_RenderFrame(refdef_t *fd);

void 		R_AppActivate(qboolean active);

// =========
//  Drawing
// =========
void		Draw_GetPicSize(int *w, int *h, char *name);
void		Draw_Pic(int x, int y, char *name);
void		Draw_StretchPic(int x, int y, int w, int h, char *name);
void		Draw_Char(int x, int y, int c);
void		Draw_CharQueue();
void		Draw_TileClear(int x, int y, int w, int h, char *name);
void		Draw_Fill(int x, int y, int w, int h, int c);
void		Draw_FadeScreen(void);
void		Draw_StretchRaw(int x, int y, int w, int h, int cols, int rows, byte *data);

// ==============
//  Registration
// ==============
void			R_BeginRegistration(char *map);
void			R_EndRegistration(void);

struct model_s	*R_RegisterModel(char *name);
struct image_s	*R_RegisterSkin(char *name);
struct image_s	*R_RegisterPic(char *name);

// ======
//  Misc
// ======
void		R_SetSky(char *name, float rotate, vec3_t axis);
void 		R_SetPalette(const byte *palette);


// ================
//  Default assets
// ================
qboolean	R_LoadDefaultAssets(void);


// =================
//  OpenGL Bindings
// =================
qboolean 	OGL_Init();
void 		OGL_Shutdown();

void		OGL_CleanupSDL();

void 		OGL_DefaultState();

void		OGL_BindShader(gl4_shader_t *shader);

/*
====================================================================

DEFAULT ASSETS

====================================================================
*/
//gl4_mesh_t		*gl4_mesh_tri;

extern gl4_image_t		*image_chars;

extern gl4_shader_t		*shader_hello_tri;
extern gl4_shader_t		*shader_draw_char;

/*
====================================================================

IMPORTED FUNCTIONS

====================================================================
*/

extern	refimport_t		ri;

#ifdef __cplusplus
};
#endif

#endif //ZEALOT_GL4_REF_H
