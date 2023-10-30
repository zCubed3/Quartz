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

#ifndef ZEALOT_QLIB_H
#define ZEALOT_QLIB_H

//============================================================================

#include "qexport.h"

//============================================================================

typedef 	void* 	qlib;

typedef 	void*(*qlib_fptr)(void*);

//============================================================================

extern const char*		qlib_postfix;
extern const char*		qlib_prefix;

//============================================================================

extern void* QLib_LoadLibrary(const char* path);

extern int QLib_UnloadLibrary(void* lib);

extern qlib_fptr QLib_GetFuncPtr(void* lib, const char* func);

//============================================================================

#endif //ZEALOT_QLIB_H
