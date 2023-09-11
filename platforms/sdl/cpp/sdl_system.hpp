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
// sdl_system.hpp
//

#ifndef ZEALOTQUAKE2_SDL_SYSTEM_HPP
#define ZEALOTQUAKE2_SDL_SYSTEM_HPP

#include "../../../game/q_shared.hpp"

#include "../../../qcommon/qexport.h"
#include "../../../qcommon/qlib.h"

namespace ZealotQ2
{
	class SDLSystem : public System
	{
	protected:
		const char		*game_name = "game"; // TODO: Allow this to be changed at runtime
		qlib			qlib_game = nullptr;

		int				hunkcount;

		unsigned char	*membase;
		int				hunkmaxsize;
		int				cursize;

	public:
		//
		// State
		//
		virtual 	void 	Init() override;
		virtual 	void 	Quit() override;

		virtual		void 	Error(const char *error, va_list va) override;

		//
		// Getters
		//
		virtual 	void*	GetGameAPI(void* parms)	override;

		virtual 	char* 	GetClipboardData() override;

		virtual 	int		GetMilliseconds() override;

		//
		// IO
		//
		virtual 	char*	FindFirst(char *path, unsigned musthave, unsigned canthave) override;
		virtual 	char*	FindNext(unsigned musthave, unsigned canthave) override;
		virtual		void	FindClose() override;

		virtual 	void 	CreateDir(const char *path) override;

		//
		// Hunk
		//
		virtual		void*	HunkAlloc(int size) override;
		virtual 	void 	HunkFree(void *buf) override;
		virtual 	void* 	HunkBegin(int maxsize) override;
		virtual		int 	HunkEnd() override;
	};

	// =======
	// Globals
	// =======
	extern	SDLSystem	sdl_system;
}

#endif //ZEALOTQUAKE2_SDL_SYSTEM_HPP
