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
// id_input_sdl.cpp -- idInput implementation for SDL
//

#include <SDL.h>

#include "id_input_sdl.hpp"

//============================================================================

extern "C" {
	#include "../../client/client.h"
	#include "sdlquake.h"
}

//============================================================================

#ifdef USE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#endif

//============================================================================

extern "C" {
	extern unsigned 	sys_msg_time;

	cvar_t	*in_mouse;
	cvar_t	*in_joystick;

	// none of these cvars are saved over a session
	// this means that advanced controller configuration needs to be executed
	// each time.  this avoids any problems with getting back to a default usage
	// or when changing from one controller to another.  this way at least something
	// works.
	cvar_t	*joy_name;
	cvar_t	*joy_advanced;
	cvar_t	*joy_advaxisx;
	cvar_t	*joy_advaxisy;
	cvar_t	*joy_advaxisz;
	cvar_t	*joy_advaxisr;
	cvar_t	*joy_advaxisu;
	cvar_t	*joy_advaxisv;
	cvar_t	*joy_forwardthreshold;
	cvar_t	*joy_sidethreshold;
	cvar_t	*joy_pitchthreshold;
	cvar_t	*joy_yawthreshold;
	cvar_t	*joy_forwardsensitivity;
	cvar_t	*joy_sidesensitivity;
	cvar_t	*joy_pitchsensitivity;
	cvar_t	*joy_yawsensitivity;
	cvar_t	*joy_upthreshold;
	cvar_t	*joy_upsensitivity;

	cvar_t	*v_centermove;
	cvar_t	*v_centerspeed;

	qboolean	joy_avail, joy_advancedinit, joy_haspov;

	qboolean	in_appactive;

	// forward-referenced functions
	void IN_StartupJoystick (void);
	void Joy_AdvancedUpdate_f (void);
	void IN_JoyMove (usercmd_t *cmd);

};

//============================================================================

idInput* 	id_in = new idInputSDL();

//============================================================================

/*
============================================================

  MOUSE CONTROL

============================================================
*/

// mouse variables
cvar_t	*m_filter;

qboolean	mlooking;

void IN_MLookDown (void) { mlooking = true; }
void IN_MLookUp (void) {
mlooking = false;
if (!freelook->value && lookspring->value)
		IN_CenterView ();
}

int			mouse_buttons;
int			mouse_oldbuttonstate;
int			mouse_x, mouse_y, old_mouse_x, old_mouse_y;

qboolean	mouseactive;	// false when not focus app

qboolean	restore_spi;
qboolean	mouseinitialized;
int			originalmouseparms[3], newmouseparms[3] = {0, 0, 1};

int			window_center_x, window_center_y;


/*
===========
IN_ActivateMouse

Called when the window gains focus or changes in some way
===========
*/

/*
===========
IN_MouseEvent
===========
*/
void IN_MouseEvent (int mstate)
{
	int		i;

	if (!mouseinitialized)
		return;

// perform button actions
	for (i=0 ; i<mouse_buttons ; i++)
	{
		if ( (mstate & (1<<i)) &&
			!(mouse_oldbuttonstate & (1<<i)) )
		{
			Key_Event (K_MOUSE1 + i, true, sys_msg_time);
		}

		if ( !(mstate & (1<<i)) &&
			(mouse_oldbuttonstate & (1<<i)) )
		{
				Key_Event (K_MOUSE1 + i, false, sys_msg_time);
		}
	}	
		
	mouse_oldbuttonstate = mstate;
}


/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove (usercmd_t *cmd)
{
	int 	current_pos_x, current_pos_y;
	int		mx, my;

	if (!mouseactive)
		return;

	// find mouse movement
	SDL_GetMouseState(&current_pos_x, &current_pos_y);

	mx = current_pos_x - window_center_x;
	my = current_pos_y - window_center_y;

#if 0
	if (!mx && !my)
		return;
#endif

	if (m_filter->value)
	{
		mouse_x = (mx + old_mouse_x) * 0.5;
		mouse_y = (my + old_mouse_y) * 0.5;
	}
	else
	{
		mouse_x = mx;
		mouse_y = my;
	}

	old_mouse_x = mx;
	old_mouse_y = my;

	mouse_x *= sensitivity->value;
	mouse_y *= sensitivity->value;

// add mouse X/Y movement to cmd
	if ( (in_strafe.state & 1) || (lookstrafe->value && mlooking ))
		cmd->sidemove += m_side->value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw->value * mouse_x;

	if ( (mlooking || freelook->value) && !(in_strafe.state & 1))
	{
		cl.viewangles[PITCH] += m_pitch->value * mouse_y;
	}
	else
	{
		cmd->forwardmove -= m_forward->value * mouse_y;
	}

	// force the mouse to the center, so there's room to move
	if (mx || my)
		SDL_WarpMouseInWindow(cl_window, window_center_x, window_center_y);
}

//============================================================================

// =============
//  Input State
// =============
void idInputSDL::Init()
{
	// mouse variables
	m_filter 				= Cvar_Get("m_filter", "0", 0);
	in_mouse 				= Cvar_Get("in_mouse", "1", CVAR_ARCHIVE);

	// joystick variables
	in_joystick 			= Cvar_Get("in_joystick", "0", CVAR_ARCHIVE);
	joy_name 				= Cvar_Get("joy_name", "joystick", 0);
	joy_advanced 			= Cvar_Get("joy_advanced", "0", 0);
	joy_advaxisx 			= Cvar_Get("joy_advaxisx", "0", 0);
	joy_advaxisy 			= Cvar_Get("joy_advaxisy", "0", 0);
	joy_advaxisz 			= Cvar_Get("joy_advaxisz", "0", 0);
	joy_advaxisr 			= Cvar_Get("joy_advaxisr", "0", 0);
	joy_advaxisu 			= Cvar_Get("joy_advaxisu", "0", 0);
	joy_advaxisv 			= Cvar_Get("joy_advaxisv", "0", 0);
	joy_forwardthreshold 	= Cvar_Get("joy_forwardthreshold", "0.15", 0);
	joy_sidethreshold 		= Cvar_Get("joy_sidethreshold", "0.15", 0);
	joy_upthreshold			= Cvar_Get("joy_upthreshold", "0.15", 0);
	joy_pitchthreshold 		= Cvar_Get("joy_pitchthreshold", "0.15", 0);
	joy_yawthreshold 		= Cvar_Get("joy_yawthreshold", "0.15", 0);
	joy_forwardsensitivity 	= Cvar_Get("joy_forwardsensitivity", "-1", 0);
	joy_sidesensitivity 	= Cvar_Get("joy_sidesensitivity", "-1", 0);
	joy_upsensitivity 		= Cvar_Get("joy_upsensitivity", "-1", 0);
	joy_pitchsensitivity 	= Cvar_Get("joy_pitchsensitivity", "1", 0);
	joy_yawsensitivity 		= Cvar_Get("joy_yawsensitivity", "-1", 0);

	// centering
	v_centermove 			= Cvar_Get("v_centermove", "0.15", 0);
	v_centerspeed 			= Cvar_Get("v_centerspeed", "500", 0);

	Cmd_AddCommand("+mlook", IN_MLookDown);
	Cmd_AddCommand("-mlook", IN_MLookUp);

	//Cmd_AddCommand("joy_advancedupdate", Joy_AdvancedUpdate_f);

	InitMouse();
	//IN_StartupJoystick();
}

void idInputSDL::Shutdown()
{
	ActivateMouse(false);
}

void idInputSDL::Activate(qboolean active)
{
	in_appactive = active;
	mouseactive = !active;		// force a new window check or turn off
}

//============================================================================

// ===============
//  Input Methods
// ===============
void idInputSDL::Commands()
{
	// TODO: Restore id's joystick code

	/*
	int i, key_index;
	DWORD buttonstate, povstate;

	if (!joy_avail)
	{
		return;
	}


	// loop through the joystick buttons
	// key a joystick event or auxillary event for higher number buttons for each state change
	buttonstate = ji.dwButtons;
	for (i = 0; i < joy_numbuttons; i++)
	{
		if ((buttonstate & (1 << i)) && !(joy_oldbuttonstate & (1 << i)))
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event(key_index + i, true, 0);
		}

		if (!(buttonstate & (1 << i)) && (joy_oldbuttonstate & (1 << i)))
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event(key_index + i, false, 0);
		}
	}
	joy_oldbuttonstate = buttonstate;

	if (joy_haspov)
	{
		// convert POV information into 4 bits of state information
		// this avoids any potential problems related to moving from one
		// direction to another without going through the center position
		povstate = 0;
		if (ji.dwPOV != JOY_POVCENTERED)
		{
			if (ji.dwPOV == JOY_POVFORWARD)
				povstate |= 0x01;
			if (ji.dwPOV == JOY_POVRIGHT)
				povstate |= 0x02;
			if (ji.dwPOV == JOY_POVBACKWARD)
				povstate |= 0x04;
			if (ji.dwPOV == JOY_POVLEFT)
				povstate |= 0x08;
		}
		// determine which bits have changed and key an auxillary event for each change
		for (i = 0; i < 4; i++)
		{
			if ((povstate & (1 << i)) && !(joy_oldpovstate & (1 << i)))
			{
				Key_Event(K_AUX29 + i, true, 0);
			}

			if (!(povstate & (1 << i)) && (joy_oldpovstate & (1 << i)))
			{
				Key_Event(K_AUX29 + i, false, 0);
			}
		}
		joy_oldpovstate = povstate;
	}
	*/
}

void idInputSDL::Frame()
{
	if (!mouseinitialized)
		return;

	if (!in_mouse || !in_appactive)
	{
		ActivateMouse(false);
		return;
	}

	if (!cl.refresh_prepped
		|| cls.key_dest == key_console
		|| cls.key_dest == key_menu)
	{
		// temporarily deactivate if in fullscreen
		if (Cvar_VariableValue("vid_fullscreen") == 0)
		{
			ActivateMouse(false);
			return;
		}
	}

	ActivateMouse(true);
}

void idInputSDL::Move(usercmd_t *cmd)
{
	IN_MouseMove(cmd);

	// TODO: Fix joysticks
	//if (ActiveApp)
	//	IN_JoyMove(cmd);
}

//============================================================================

// =============
//  SDL Methods
// =============

// TODO: Organize
qboolean	mwheel_delay = false;

void idInputSDL::ActivateMouse(qboolean active)
{
	int		width, height;

	if (!mouseinitialized)
		return;

	if (!in_mouse->value)
	{
		mouseactive = false;
		return;
	}

	mouseactive = active;

	// If the mouse is activated, warp to the center of the window
	if (mouseactive)
	{
		SDL_GetWindowSize(cl_window, &width, &height);

		window_center_x = width / 2;
		window_center_y = height / 2;

		// TODO: Capture the cursor?
		//SDL_WarpMouseInWindow(cl_window, window_center_x, window_center_y);
		SDL_ShowCursor(false);
	}
	else // Otherwise, just make it visible
	{
		SDL_ShowCursor(true);
	}
}

void idInputSDL::PollSDL()
{
	SDL_Event 	event;
	int 		event_time;

	event_time = Sys_Milliseconds();

	if (mwheel_delay)
	{
		mwheel_delay = false;
		Key_Event(K_MWHEELUP, false, event_time);
		Key_Event(K_MWHEELDOWN, false, event_time);
	}

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			qboolean pressed = event.key.state == SDL_PRESSED;
			int key = MapSDLKey(event.key.keysym.sym);

			if (key != -1)
				Key_Event(key, pressed, event_time);
		}

		if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		{
			qboolean pressed = event.button.state == SDL_PRESSED;

			// TODO: Fix out of bounds for MOUSE4 and MOUSE5
			int button = MapSDLMouseButton(event.button.button);

			if (button != -1)
				Key_Event(button, pressed, event_time);
		}

		if (event.type == SDL_WINDOWEVENT)
		{
			if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
				Activate(true);

			if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
				Activate(false);
		}

		if (event.type == SDL_MOUSEWHEEL)
		{
			// Allow bhop, delay the mwheel up release by one frame

			if (event.wheel.y > 0)
			{
				mwheel_delay = true;
				Key_Event(K_MWHEELUP, true, event_time);
			}

			if (event.wheel.y < 0)
			{
				mwheel_delay = true;
				Key_Event(K_MWHEELDOWN, true, event_time);
			}
		}

#ifdef USE_IMGUI
		// If the game is active do not give ImGui any input
		if (!mouseactive)
			ImGui_ImplSDL2_ProcessEvent(&event);
#endif
	}

	// TODO: Find a better home for this
#ifdef USE_IMGUI
	if (mouseactive)
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
#endif
}

//============================================================================

// =============
//  Input State
// =============
void idInputSDL::InitMouse()
{
	cvar_t *cv;

	cv = Cvar_Get("in_initmouse", "1", CVAR_NOSET);
	if (!cv->value)
		return;

	mouseinitialized = true;
	mouse_buttons = 5;
}

//============================================================================

// =========
//  Helpers
// =========

int idInputSDL::MapSDLKey(SDL_Keycode code)
{
	// TODO: Handle foreign keyboards
	switch (code)
	{
		// ============
		// Special keys
		// ============
		case SDLK_MINUS:
			return '-';

		case SDLK_EQUALS:
			return '=';

		case SDLK_BACKQUOTE:
			return '~';

		case SDLK_BACKSPACE:
			return K_BACKSPACE;

		case SDLK_RETURN:
			return K_ENTER;

		case SDLK_SPACE:
			return K_SPACE;

		case SDLK_ESCAPE:
			return K_ESCAPE;

		case SDLK_PAUSE:
			return K_PAUSE;

		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
			return K_SHIFT;

			// ==========
			// Arrow keys
			// ==========
		case SDLK_LEFT:
			return K_LEFTARROW;

		case SDLK_RIGHT:
			return K_RIGHTARROW;

		case SDLK_DOWN:
			return K_DOWNARROW;

		case SDLK_UP:
			return K_UPARROW;
	}

	// =================
	// Ascii range keys
	// =================
	if (code >= SDLK_EXCLAIM && code <= SDLK_z)
	{
		return (int)code;
	}

	// ==========
	// Function keys
	// ==========
	if (code >= SDLK_F1 && code <= SDLK_F12)
	{
		int off = code - SDLK_F1;
		return (int)K_F1 + off;
	}

	// =========
	// Error key
	// =========
	return -1;
}

int idInputSDL::MapSDLMouseButton(int index)
{
	int 	actual;

	actual = index - 1;

	return K_MOUSE1 + actual;
}

//============================================================================

//
// Legacy joystick code (needs to be ported / updated to SDL)
//

#if 0

/*
=========================================================================

JOYSTICK

=========================================================================
*/

/*
===============
IN_StartupJoystick
===============
*/
void IN_StartupJoystick (void)
{
	int			numdevs;
	JOYCAPS		jc;
	MMRESULT	mmr;
	cvar_t		*cv;

 	// assume no joystick
	joy_avail = false;

	// abort startup if user requests no joystick
	cv = Cvar_Get ("in_initjoy", "1", CVAR_NOSET);
	if ( !cv->value )
		return;

	// verify joystick driver is present
	if ((numdevs = joyGetNumDevs ()) == 0)
	{
//		Com_Printf ("\njoystick not found -- driver not present\n\n");
		return;
	}

	// cycle through the joystick ids for the first valid one
	for (joy_id=0 ; joy_id<numdevs ; joy_id++)
	{
		memset (&ji, 0, sizeof(ji));
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNCENTERED;

		if ((mmr = joyGetPosEx (joy_id, &ji)) == JOYERR_NOERROR)
			break;
	}

	// abort startup if we didn't find a valid joystick
	if (mmr != JOYERR_NOERROR)
	{
		Com_Printf ("\njoystick not found -- no valid joysticks (%x)\n\n", mmr);
		return;
	}

	// get the capabilities of the selected joystick
	// abort startup if command fails
	memset (&jc, 0, sizeof(jc));
	if ((mmr = joyGetDevCaps (joy_id, &jc, sizeof(jc))) != JOYERR_NOERROR)
	{
		Com_Printf ("\njoystick not found -- invalid joystick capabilities (%x)\n\n", mmr);
		return;
	}

	// save the joystick's number of buttons and POV status
	joy_numbuttons = jc.wNumButtons;
	joy_haspov = jc.wCaps & JOYCAPS_HASPOV;

	// old button and POV states default to no buttons pressed
	joy_oldbuttonstate = joy_oldpovstate = 0;

	// mark the joystick as available and advanced initialization not completed
	// this is needed as cvars are not available during initialization

	joy_avail = true;
	joy_advancedinit = false;

	Com_Printf ("\njoystick detected\n\n");
}


/*
===========
RawValuePointer
===========
*/
PDWORD RawValuePointer (int axis)
{
	switch (axis)
	{
	case JOY_AXIS_X:
		return &ji.dwXpos;
	case JOY_AXIS_Y:
		return &ji.dwYpos;
	case JOY_AXIS_Z:
		return &ji.dwZpos;
	case JOY_AXIS_R:
		return &ji.dwRpos;
	case JOY_AXIS_U:
		return &ji.dwUpos;
	case JOY_AXIS_V:
		return &ji.dwVpos;
	}
}


/*
===========
Joy_AdvancedUpdate_f
===========
*/
void Joy_AdvancedUpdate_f (void)
{

	// called once by IN_ReadJoystick and by user whenever an update is needed
	// cvars are now available
	int	i;
	DWORD dwTemp;

	// initialize all the maps
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		dwAxisMap[i] = AxisNada;
		dwControlMap[i] = JOY_ABSOLUTE_AXIS;
		pdwRawValue[i] = RawValuePointer(i);
	}

	if( joy_advanced->value == 0.0)
	{
		// default joystick initialization
		// 2 axes only with joystick control
		dwAxisMap[JOY_AXIS_X] = AxisTurn;
		// dwControlMap[JOY_AXIS_X] = JOY_ABSOLUTE_AXIS;
		dwAxisMap[JOY_AXIS_Y] = AxisForward;
		// dwControlMap[JOY_AXIS_Y] = JOY_ABSOLUTE_AXIS;
	}
	else
	{
		if (strcmp (joy_name->string, "joystick") != 0)
		{
			// notify user of advanced controller
			Com_Printf ("\n%s configured\n\n", joy_name->string);
		}

		// advanced initialization here
		// data supplied by user via joy_axisn cvars
		dwTemp = (DWORD) joy_advaxisx->value;
		dwAxisMap[JOY_AXIS_X] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_X] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisy->value;
		dwAxisMap[JOY_AXIS_Y] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Y] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisz->value;
		dwAxisMap[JOY_AXIS_Z] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Z] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisr->value;
		dwAxisMap[JOY_AXIS_R] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_R] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisu->value;
		dwAxisMap[JOY_AXIS_U] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_U] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD) joy_advaxisv->value;
		dwAxisMap[JOY_AXIS_V] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_V] = dwTemp & JOY_RELATIVE_AXIS;
	}

	// compute the axes to collect from DirectInput
	joy_flags = JOY_RETURNCENTERED | JOY_RETURNBUTTONS | JOY_RETURNPOV;
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		if (dwAxisMap[i] != AxisNada)
		{
			joy_flags |= dwAxisFlags[i];
		}
	}
}


/*
===============
IN_ReadJoystick
===============
*/
qboolean IN_ReadJoystick (void)
{

	memset(&ji, 0, sizeof(ji));
	ji.dwSize = sizeof(ji);
	ji.dwFlags = joy_flags;

	if (joyGetPosEx(joy_id, &ji) == JOYERR_NOERROR)
	{
		return true;
	}
	else
	{
		// read error occurred
		// turning off the joystick seems too harsh for 1 read error,\
		// but what should be done?
		// Com_Printf ("IN_ReadJoystick: no response\n");
		// joy_avail = false;
		return false;
	}
}

/*
===========
IN_JoyMove
===========
*/
void IN_JoyMove (usercmd_t *cmd)
{
	float	speed, aspeed;
	float	fAxisValue;
	int		i;

	// complete initialization if first time in
	// this is needed as cvars are not available at initialization time
	if( joy_advancedinit != true )
	{
		Joy_AdvancedUpdate_f();
		joy_advancedinit = true;
	}

	// verify joystick is available and that the user wants to use it
	if (!joy_avail || !in_joystick->value)
	{
		return;
	}

	// collect the joystick data, if possible
	if (IN_ReadJoystick () != true)
	{
		return;
	}

	if ( (in_speed.state & 1) ^ (int)cl_run->value)
		speed = 2;
	else
		speed = 1;
	aspeed = speed * cls.frametime;

	// loop through the axes
	for (i = 0; i < JOY_MAX_AXES; i++)
	{
		// get the floating point zero-centered, potentially-inverted data for the current axis
		fAxisValue = (float) *pdwRawValue[i];
		// move centerpoint to zero
		fAxisValue -= 32768.0;

		// convert range from -32768..32767 to -1..1
		fAxisValue /= 32768.0;

		switch (dwAxisMap[i])
		{
		case AxisForward:
			if ((joy_advanced->value == 0.0) && mlooking)
			{
				// user wants forward control to become look control
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{
					// if mouse invert is on, invert the joystick pitch value
					// only absolute control support here (joy_advanced is false)
					if (m_pitch->value < 0.0)
					{
						cl.viewangles[PITCH] -= (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
				}
			}
			else
			{
				// user wants forward control to be forward control
				if (fabs(fAxisValue) > joy_forwardthreshold->value)
				{
					cmd->forwardmove += (fAxisValue * joy_forwardsensitivity->value) * speed * cl_forwardspeed->value;
				}
			}
			break;

		case AxisSide:
			if (fabs(fAxisValue) > joy_sidethreshold->value)
			{
				cmd->sidemove += (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
			}
			break;

		case AxisUp:
			if (fabs(fAxisValue) > joy_upthreshold->value)
			{
				cmd->upmove += (fAxisValue * joy_upsensitivity->value) * speed * cl_upspeed->value;
			}
			break;

		case AxisTurn:
			if ((in_strafe.state & 1) || (lookstrafe->value && mlooking))
			{
				// user wants turn control to become side control
				if (fabs(fAxisValue) > joy_sidethreshold->value)
				{
					cmd->sidemove -= (fAxisValue * joy_sidesensitivity->value) * speed * cl_sidespeed->value;
				}
			}
			else
			{
				// user wants turn control to be turn control
				if (fabs(fAxisValue) > joy_yawthreshold->value)
				{
					if(dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * aspeed * cl_yawspeed->value;
					}
					else
					{
						cl.viewangles[YAW] += (fAxisValue * joy_yawsensitivity->value) * speed * 180.0;
					}

				}
			}
			break;

		case AxisLook:
			if (mlooking)
			{
				if (fabs(fAxisValue) > joy_pitchthreshold->value)
				{
					// pitch movement detected and pitch movement desired by user
					if(dwControlMap[i] == JOY_ABSOLUTE_AXIS)
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * aspeed * cl_pitchspeed->value;
					}
					else
					{
						cl.viewangles[PITCH] += (fAxisValue * joy_pitchsensitivity->value) * speed * 180.0;
					}
				}
			}
			break;

		default:
			break;
		}
	}
}

#endif
