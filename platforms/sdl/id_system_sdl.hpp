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
// id_system_sdl.hpp -- idSystem implementation for SDL
//

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

#ifndef ZEALOT_ID_SYSTEM_SDL_HPP
#define ZEALOT_ID_SYSTEM_SDL_HPP

#include "../../qcommon/id_system.hpp"

#include "../../qcore/qlib.h"

class idSystemSDL : public idSystem
{
protected:
	// ==============
	//  Game Library
	// ==============
	qlib 	qlib_game = nullptr;

public:
	// ==============
	//  System State
	// ==============
	virtual void	Init() override;
	virtual void	Quit() override;

	virtual void	AppActivate() override;

	virtual void	Error(char *error, va_list argptr) override;

	// =================
	//  Game Management
	// =================
	virtual void	UnloadGame() override;
	virtual void*	GetGameAPI(void *parms) override;

	// ============
	//  Console IO
	// ============
	virtual char*	ConsoleInput() override;
	virtual void	ConsoleOutput(char *string) override;

	virtual void	SendKeyEvents() override;

	// ===========
	//  System IO
	// ===========
	virtual char*	GetClipboardData() override;

	virtual void 	GetCurrentDir(char *string, long size) override;
};

#endif //ZEALOT_SDL_SYSTEM_HPP
