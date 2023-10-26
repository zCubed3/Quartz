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
// gl4_rreg.cpp - Implementations of registration functions
//

#include "gl4_ref.hpp"

#include "gl4_model.hpp"

#include <glad/glad.h>

//
// R_BeginRegistration
//
void R_BeginRegistration(char *map)
{

}

//
// R_EndRegistration
//
void R_EndRegistration(void)
{

}

//
// R_RegisterModel
//

struct model_s *R_RegisterModel(char *name)
{
	struct model_s* mod;
	char*			buf;
	int				i;
	int 			modfilelen;

	if (!name[0])
		ri.Sys_Error (ERR_DROP, "Mod_ForName: NULL name");

	//
	// inline models are grabbed only from worldmodel
	//

	// TODO
	if (name[0] == '*')
	{
		return nullptr;

		/*
		i = atoi(name+1);
		if (i < 1 || !r_worldmodel || i >= r_worldmodel->numsubmodels)
			ri.Sys_Error (ERR_DROP, "bad inline model number");

		return &mod_inline[i];
		 */
	}

	//
	// search the currently loaded models
	//
	/*
	for (i=0 , mod=mod_known ; i<mod_numknown ; i++, mod++)
	{
		if (!mod->name[0])
			continue;
		if (!strcmp (mod->name, name) )
			return mod;
	}

	//
	// find a free model slot spot
	//
	for (i=0 , mod=mod_known ; i<mod_numknown ; i++, mod++)
	{
		if (!mod->name[0])
			break;	// free spot
	}
	if (i == mod_numknown)
	{
		if (mod_numknown == MAX_MOD_KNOWN)
			ri.Sys_Error (ERR_DROP, "mod_numknown == MAX_MOD_KNOWN");
		mod_numknown++;
	}
	*/

	mod = new model_t;
	strcpy (mod->name, name);

	//
	// load the file
	//
	modfilelen = ri.FS_LoadFile (mod->name, (void**)&buf);

	if (buf == nullptr)
	{
		//if (crash)
		//	ri.Sys_Error (ERR_DROP, "Mod_NumForName: %s not found", mod->name);


		memset (mod->name, 0, sizeof(mod->name));
		return nullptr;
	}

	//
	// fill it in
	//

	// call the apropriate loader

	switch (LittleLong(*(unsigned *)buf))
	{
		case IDALIASHEADER:
			Mod_LoadAliasModel(mod, buf);
			break;

		default:
			return nullptr;
	}

	ri.FS_FreeFile(buf);

	return mod;
}

//
// R_RegisterSkin
//
struct image_s *R_RegisterSkin(char *name)
{
	return NULL;
}

//
// R_RegisterPic
//
struct image_s *R_RegisterPic(char *name)
{
	return Draw_FindPic(name);
}