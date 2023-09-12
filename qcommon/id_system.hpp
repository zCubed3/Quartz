/*
Copyright (C) 1997-2001 Id Software, Inc., 2023 zCubed3 (Liam R.)

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
// id_system.hpp -- idSystem declaration
//

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

#ifndef ZEALOT_ID_SYSTEM_HPP
#define ZEALOT_ID_SYSTEM_HPP

class idSystem
{
public:
	// ==============
	//  System State
	// ==============
	virtual void	Init() = 0;
	virtual void	Quit() = 0;

	virtual void	AppActivate() = 0;

	virtual void	Error(char *error, va_list argptr) = 0;

	// =================
	//  Game Management
	// =================
	virtual void	UnloadGame() = 0;
	virtual void*	GetGameAPI(void *parms) = 0;

	// ============
	//  Console IO
	// ============
	virtual char*	ConsoleInput() = 0;
	virtual void	ConsoleOutput(char *string) = 0;

	virtual void	SendKeyEvents() = 0;

	// ===========
	//  System IO
	// ===========
	virtual char*	GetClipboardData() = 0;

	virtual void 	GetCurrentDir(char *string, long size) = 0;
};

extern idSystem *id_sys;

#endif //ZEALOT_ID_SYSTEM_HPP
