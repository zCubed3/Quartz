/*
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
// qlib.c
//

#include "qlib.h"

#ifdef WIN32

#include <Windows.h>

//============================================================================

const char*		qlib_postfix = ".dll";
const char*		qlib_prefix = "";

//============================================================================

qlib QLib_LoadLibrary(const char* path)
{
	return LoadLibrary(path);
}

int QLib_UnloadLibrary(qlib lib)
{
	return FreeLibrary((HMODULE)lib);
}

qlib_fptr QLib_GetFuncPtr(qlib lib, const char* func)
{
	return (qlib_fptr)GetProcAddress((HMODULE)lib, func);
}

//============================================================================

#endif

//============================================================================

#if defined(__linux__)

const char*		qlib_postfix = ".so";

#endif

#if defined(__linux__) || defined(__APPLE__)

const char*		qlib_prefix = "lib/lib";

#include <dlfcn.h>
#include <stdlib.h>

qlib QLib_LoadLibrary(const char* path)
{
	return dlopen(path, RTLD_LAZY);
}

int QLib_UnloadLibrary(qlib lib)
{
	if (lib == NULL)
		return 1;

	return dlclose(lib) == 0;
}

qlib_fptr QLib_GetFuncPtr(qlib lib, const char* func)
{
	return dlsym(lib, func);
}

#endif

//============================================================================

#ifdef __APPLE__

//============================================================================

const char* 	qlib_postfix = ".dylib";

//============================================================================

#endif