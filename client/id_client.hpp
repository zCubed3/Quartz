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
// id_client.hpp -- idClient declaration (formerly certain loose CL_ functions)
//

#ifndef ZEALOT_ID_CLIENT_HPP
#define ZEALOT_ID_CLIENT_HPP

#ifndef __cplusplus
#error "Tried including C++ header inside of a C source file!"
#endif

//============================================================================

class idClient
{
public:
	// ==============
	//  Client State
	// ==============
	void 				Init();
	void 				Shutdown();

	void				RunFrame(int msec);

	// =======
	//  Input
	// =======
	struct usercmd_s	CreateCmd();

protected:
	// ==============
	//  Client State
	// ==============
	void				InitLocal();

	// ============
	//  Networking
	// ============
	void				ReadPackets();
	void				SendCommand();

	// Actually sends a command (the full named function does safety checking)
	void 				SendCmd();

	// ============
	//  Dear ImGui
	// ============

	// Not ifdef'ed because it's stubbed on non USE_IMGUI builds
	// TODO: This is sloppy, please for the love of all that's good replace it at some point
	static void			StaticDrawImGui();
};

//============================================================================

extern idClient*	id_cl;

//============================================================================

#endif //ZEALOT_ID_CLIENT_HPP
