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
// id_input_sdl.hpp -- idInput implementation for SDL
//

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

#ifndef ZEALOT_ID_INPUT_SDL_HPP
#define ZEALOT_ID_INPUT_SDL_HPP

//============================================================================

#include "../../client/id_input.hpp"

#include <SDL.h>

//============================================================================

class idInputSDL : public idInput
{
public:
	// =============
	//  Input State
	// =============
	virtual void 	Init() override;
	virtual void 	Shutdown() override;

	virtual void 	Activate(qboolean active) override;

	// ===============
	//  Input Methods
	// ===============
	virtual void 	Commands() override;

	virtual void 	Frame() override;

	virtual void 	Move(usercmd_t *cmd) override;

	// =============
	//  SDL Methods
	// =============
	void			ActivateMouse(qboolean active);
	void			PollSDL();

protected:
	// =============
	//  Input State
	// =============
	void 			InitMouse();

	// =========
	//  Helpers
	// =========

	// Remaps an SDL v-keycode into a Quake keycode
	int 			MapSDLKey(SDL_Keycode code);

	// Remaps an SDL mouse button into a Quake keycode
	int 			MapSDLMouseButton(int index);
};

//============================================================================

#endif //ZEALOT_ID_INPUT_SDL_HPP
