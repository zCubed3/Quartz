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

#include "../gl4_image.hpp"

#include <glad/glad.h>

#include "../../zealcore/zeal_stack.hpp"

//============================================================================

void LoadPCX(const char *filename, pcx_info_t* info)
{
	byte* raw;
	pcx_t* pcx;
	int x, y;
	int len;
	int dataByte, runLength;
	byte* pixels;

	info->pixels = nullptr;
	info->palette = nullptr;

	//
	// load the file
	//
	len = ri.FS_LoadFile(filename, (void**) &raw);
	if (!raw)
	{
		ri.Con_Printf(PRINT_DEVELOPER, "Bad pcx file %s\n", filename);
		return;
	}

	//
	// parse the PCX file
	//
	pcx = (pcx_t*) raw;

	pcx->xmin = LittleShort(pcx->xmin);
	pcx->ymin = LittleShort(pcx->ymin);
	pcx->xmax = LittleShort(pcx->xmax);
	pcx->ymax = LittleShort(pcx->ymax);
	pcx->hres = LittleShort(pcx->hres);
	pcx->vres = LittleShort(pcx->vres);
	pcx->bytes_per_line = LittleShort(pcx->bytes_per_line);
	pcx->palette_type = LittleShort(pcx->palette_type);

	raw = &pcx->data;

	if (pcx->manufacturer != 0x0a
		|| pcx->version != 5
		|| pcx->encoding != 1
		|| pcx->bits_per_pixel != 8
		|| pcx->xmax >= 640
		|| pcx->ymax >= 480)
	{
		ri.Con_Printf(PRINT_ALL, "Bad pcx file %s\n", filename);
		return;
	}

	info->pixels 	= new byte[(pcx->ymax + 1) * (pcx->xmax + 1)];
	info->palette	= new byte[768];

	memcpy(info->palette, (byte*) pcx + len - 768, 768);

	info->width 	= pcx->xmax + 1;
	info->height 	= pcx->ymax + 1;

	// Iteration pixels
	pixels = info->pixels;

	for (y = 0; y <= pcx->ymax; y++, pixels += pcx->xmax + 1)
	{
		for (x = 0; x <= pcx->xmax;)
		{
			dataByte = *raw++;

			if ((dataByte & 0xC0) == 0xC0)
			{
				runLength = dataByte & 0x3F;
				dataByte = *raw++;
			} else
				runLength = 1;

			while (runLength-- > 0)
				pixels[x++] = dataByte;
		}
	}

	if (raw - (byte*) pcx > len)
	{
		ri.Con_Printf(PRINT_DEVELOPER, "PCX file %s was malformed", filename);

		delete[] info->pixels;
		delete[] info->palette;

		info->pixels = nullptr;
		info->palette = nullptr;
	}

	ri.FS_FreeFile(pcx);
}

void ReleasePCX(pcx_info_t* info)
{
	delete[] info->palette;
	delete[] info->pixels;

	info->palette 	= nullptr;
	info->pixels 	= nullptr;
}

//============================================================================

zealStack<image_t*> image_cache;

image_t* OGL_FindImage(const char* name)
{
	zealNode<image_t*>* 	iter;

	size_t 	len;

	if (!name)
		return nullptr;    //	ri.Sys_Error (ERR_DROP, "GL_FindImage: NULL name");

	len = strlen(name);

	if (len < 5)
		return nullptr;    //	ri.Sys_Error (ERR_DROP, "GL_FindImage: bad name: %s", name);

	// Check if it's in our cache already
	for (iter = image_cache.Top(); iter != nullptr; iter = iter->next)
		if (!strcmp(name, iter->element->name))
			return iter->element;

	// Otherwise load it!
	if (!strcmp(name+len-4, ".pcx"))
	{
		image_t* 	image;
		pcx_info_t 	pcx_info;

		LoadPCX(name, &pcx_info);

		if (!pcx_info.palette || !pcx_info.pixels)
			return nullptr; // ri.Sys_Error (ERR_DROP, "GL_FindImage: can't load %s", name);

		image = OGL_LoadPCX(&pcx_info);
		ReleasePCX(&pcx_info);

		// Cache the image
		image_cache.Push(image);

		return image;
	}

	return nullptr;
}

void OGL_BindImage(image_t* image)
{
	if (image != nullptr)
	{
		// TODO: Store the texture type!
		glBindTexture(GL_TEXTURE_2D, image->handle);
	}
	else
	{
		ri.Sys_Error(ERR_FATAL, "Tried to bind a null image!");
	}
}

image_t* OGL_LoadPCX(pcx_info_t* info)
{
	image_t*	image;
	byte*		recon_pic; // Reconstructed data

	image = new image_t;

	image->width = info->width;
	image->height = info->height;

	// Allocate OpenGL data
	glGenTextures(1, &image->handle);

	// We then need to convert the palette image to an RGB image
	recon_pic = new byte[image->width * image->height * 4];

	for (int y = 0; y < image->height; y++)
	{
		for (int x = 0; x < image->width; x++)
		{
			int 		recon_pt, pic_pt;
			int 		p_index;
			qboolean 	is_alpha;

			recon_pt 	= (y * image->width * 4) + (x * 4);
			pic_pt 		= (y * image->width) + x;

			// Get our actual pic pixel
			is_alpha 	= info->pixels[pic_pt] == 255;
			p_index 	= info->pixels[pic_pt] * 3;

			if (is_alpha)
			{
				recon_pic[recon_pt + 3] = 0x00;
			}
			else
			{
				recon_pic[recon_pt] 	= info->palette[p_index];
				recon_pic[recon_pt + 1] = info->palette[p_index + 1];
				recon_pic[recon_pt + 2] = info->palette[p_index + 2];
				recon_pic[recon_pt + 3] = 0xFF;
			}
		}
	}

	// Upload our data to the GPU
	glBindTexture(GL_TEXTURE_2D, image->handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, recon_pic);

	// Generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// TODO: Let the user decide the global filter type?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] recon_pic;

	return image;
}