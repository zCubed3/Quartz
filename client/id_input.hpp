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
// id_input.hpp -- idInput declaration
//

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

#ifndef ZEALOT_ID_INPUT_HPP
#define ZEALOT_ID_INPUT_HPP

//============================================================================

#include "../games/quake2/q_shared.hpp"

//============================================================================

class idInput
{
public:
	// =============
	//  Input State
	// =============
	virtual void 	Init() = 0;
	virtual void 	Shutdown() = 0;

	virtual void 	Activate(qboolean active) = 0;

	// ===============
	//  Input Methods
	// ===============

	// Allows devices to insert commands into the command buffer
	virtual void 	Commands() = 0;

	// Advances a frame
	virtual void 	Frame() = 0;

	// add additional movement on top of the keyboard move cmd
	virtual void 	Move(usercmd_t *cmd) = 0;
};

//============================================================================

extern idInput*		id_in;

//============================================================================

#endif //ZEALOT_ID_INPUT_HPP
