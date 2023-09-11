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
// q_shared.hpp
//

#ifndef ZEALOTQUAKE2_Q_SHARED_HPP
#define ZEALOTQUAKE2_Q_SHARED_HPP

#include <cstdlib>

#ifndef __cplusplus
#error "Only include HPP files inside of C++!"
#endif

namespace ZealotQ2 {
	class System {
	public:
		//
		// State
		//
		virtual 	void 	Init() = 0;
		virtual 	void 	Quit() = 0;

		virtual		void 	Error(const char *error, va_list va) = 0;

		//
		// Getters
		//
		virtual 	void*	GetGameAPI(void *parms) = 0;

		virtual 	char* 	GetClipboardData() = 0;

		virtual 	int		GetMilliseconds() = 0;

		//
		// IO
		//
		virtual 	char*	FindFirst(char *path, unsigned musthave, unsigned canthave) = 0;
		virtual 	char*	FindNext(unsigned musthave, unsigned canthave) = 0;
		virtual		void	FindClose() = 0;

		virtual 	void 	CreateDir(const char *path) = 0;

		//
		// Hunk
		//
		virtual		void*	HunkAlloc(int size) = 0;
		virtual 	void 	HunkFree(void *buf) = 0;
		virtual 	void* 	HunkBegin(int maxsize) = 0;
		virtual		int 	HunkEnd() = 0;
	};
}

#endif //ZEALOTQUAKE2_Q_SHARED_HPP
