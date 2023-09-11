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
// qlib.h
//
// This is a wrapper around HINSTANCE / void* dynamic linking
//

#ifndef ZEALOTQUAKE2_SHARED_LIB_H
#define ZEALOTQUAKE2_SHARED_LIB_H

#include "qexport.h"

#ifdef __cplusplus

extern "C" {

#endif

typedef	void (*qlib_fptr)();
typedef void* (*qlib_mod_fptr)(void*);

#if defined(WIN32)

#include <Windows.h>

typedef HMODULE 	qlib;

#elif defined(__linux__)

#include <dl.h>
typedef void* 		qlib;

#endif


Q_EXTERN_VAR	const char* 		qlib_postfix;


Q_EXTERN_FUNC	qlib 		QLib_LoadLibrary(const char* path);

Q_EXTERN_FUNC 	int 		QLib_UnloadLibrary(qlib lib);

Q_EXTERN_FUNC 	qlib_fptr 	QLib_GetFuncPtr(qlib lib, const char* func);

#ifdef __cplusplus
};
#endif

#endif //ZEALOTQUAKE2_SHARED_LIB_H
