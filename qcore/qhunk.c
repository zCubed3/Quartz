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
// qhunk.c - Hunk allocation
//

#include "qhunk.h"

#include <stdlib.h>
#include <string.h>

#include "qsys.h"

//============================================================================

int				hunkcount;

unsigned char	*membase;
int				hunkmaxsize;
int				cursize;

//============================================================================

void *Hunk_Begin(int maxsize)
{
	// reserve a huge chunk of memory, but don't commit any yet
	cursize = 0;
	hunkmaxsize = maxsize;

	membase = malloc(maxsize);
	memset(membase, 0, maxsize);

	if (!membase)
		Sys_Error("VirtualAlloc reserve failed");

	return (void *) membase;
}

void *Hunk_Alloc(int size)
{
	// round to cacheline
	size = (size + 31) & ~31;

	cursize += size;
	if (cursize > hunkmaxsize)
		Sys_Error("Hunk_Alloc overflow");

	return (void *) (membase + cursize - size);
}

int Hunk_End(void)
{
	hunkcount++;

	return cursize;
}

void Hunk_Free(void *base)
{
	if (base)
		free(base);

	hunkcount--;
}

//===============================================================================

