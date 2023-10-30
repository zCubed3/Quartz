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
// id_client.cpp -- idClient implementation (formerly loose CL_ functions)
//

#include "id_client.hpp"

#include "id_input.hpp"

//============================================================================

#include "client.h"

//============================================================================

#ifdef USE_IMGUI
#include <imgui.h>
#include <imgui_internal.h>
#endif

//============================================================================

extern cvar_t*		cl_maxfps;
extern cvar_t*		freelook;

extern cvar_t*		adr0;
extern cvar_t*		adr1;
extern cvar_t*		adr2;
extern cvar_t*		adr3;
extern cvar_t*		adr4;
extern cvar_t*		adr5;
extern cvar_t*		adr6;
extern cvar_t*		adr7;
extern cvar_t*		adr8;

extern cvar_t*		cl_stereo_separation;
extern cvar_t*		cl_stereo;

extern cvar_t*		rcon_client_password;
extern cvar_t*		rcon_address;

extern cvar_t*		cl_noskins;
extern cvar_t*		cl_autoskins;
extern cvar_t*		cl_footsteps;
extern cvar_t*		cl_timeout;
extern cvar_t*		cl_predict;
//extern cvar_t		*cl_minfps;
extern cvar_t*		cl_maxfps;
extern cvar_t*		cl_gun;

extern cvar_t*		cl_add_particles;
extern cvar_t*		cl_add_lights;
extern cvar_t*		cl_add_entities;
extern cvar_t*		cl_add_blend;

extern cvar_t*		cl_shownet;
extern cvar_t*		cl_showmiss;
extern cvar_t*		cl_showclamp;

extern cvar_t*		cl_paused;
extern cvar_t*		cl_timedemo;

extern cvar_t*		lookspring;
extern cvar_t*		lookstrafe;
extern cvar_t*		sensitivity;

extern cvar_t*		m_pitch;
extern cvar_t*		m_yaw;
extern cvar_t*		m_forward;
extern cvar_t*		m_side;

extern cvar_t*		cl_lightlevel;

extern cvar_t*		cl_nodelta;

//
// userinfo
//
extern cvar_t*		info_password;
extern cvar_t*		info_spectator;
extern cvar_t*		name;
extern cvar_t*		skin;
extern cvar_t*		rate;
extern cvar_t*		fov;
cvar_t*				v_fov;
extern cvar_t*		msg;
extern cvar_t*		hand;
extern cvar_t*		gender;
extern cvar_t*		gender_auto;

extern cvar_t*		cl_vwep;

//
// imgui
//
#ifdef USE_IMGUI

cvar_t*				imgui_visible;
cvar_t*				imgui_showdemo;

#endif

extern client_static_t	cls;
extern client_state_t	cl;

extern centity_t		cl_entities[MAX_EDICTS];

extern entity_state_t	cl_parse_entities[MAX_PARSE_ENTITIES];

extern cvar_t 	*allow_download;
extern cvar_t 	*allow_download_players;
extern cvar_t 	*allow_download_models;
extern cvar_t 	*allow_download_sounds;
extern cvar_t 	*allow_download_maps;

extern unsigned	sys_frame_time;
extern unsigned	frame_msec;
extern unsigned	old_sys_frame_time;

// TODO: Don't forward declare this
extern void 	CL_ConnectionlessPacket(void);
extern void		CL_FixCvarCheats(void);
extern void		CL_CheckForResend(void);
extern void		CL_WriteConfiguration(void);

extern void 	CL_ForwardToServer_f(void);
extern void 	CL_Pause_f(void);
extern void 	CL_Skins_f(void);
extern void 	CL_Userinfo_f(void);
extern void 	CL_Changing_f(void);
extern void 	CL_Connect_f(void);
extern void 	CL_Reconnect_f(void);
extern void 	CL_Rcon_f(void);
extern void 	CL_Setenv_f(void);
extern void 	CL_Precache_f(void);
extern void		CL_FinishMove(usercmd_t* cmd);

//============================================================================

idClient* 	id_cl = new idClient();

//============================================================================

#define MAX_DATA_COUNT 128

float	timings_cl_frame[MAX_DATA_COUNT];
float	timings_ref[MAX_DATA_COUNT];

//============================================================================

// ==============
//  Client State
// ==============
void idClient::Init()
{
	if (dedicated->value)
		return;        // nothing running on the client

	// all archived variables will now be loaded

	Con_Init();

	VID_Init();
	S_Init();    // sound must be initialized after window is created

	V_Init();

	net_message.data = net_message_buffer;
	net_message.maxsize = sizeof(net_message_buffer);

	M_Init();

	SCR_Init();
	cls.disable_screen = true;    // don't draw yet

	CDAudio_Init();

	InitLocal();

	id_in->Init();

//	Cbuf_AddText ("exec autoexec.cfg\n");
	FS_ExecAutoexec();
	Cbuf_Execute();
}

// id's fixme below
// FIXME: this is a callback from Sys_Quit and Com_Error.  It would be better
void idClient::Shutdown()
{
	static qboolean isdown = false;

	if (isdown)
	{
		printf("[idClient]: recursive shutdown\n");
		return;
	}
	isdown = true;

	CL_WriteConfiguration();

	CDAudio_Shutdown();
	S_Shutdown();
	id_in->Shutdown();
	VID_Shutdown();
}

void idClient::RunFrame(int msec)
{
	static int extratime;
	static int lasttimecalled;

	int			time_begin, time_end;

	if (host_speeds->value)
		time_begin = Sys_Milliseconds();
	else
		time_begin = 0; // Prevents VC++ Runtime error

	if (dedicated->value)
		return;

	extratime += msec;

	if (!cl_timedemo->value)
	{
		if (cls.state == ca_connected && extratime < 100)
			return;            // don't flood packets out while connecting

		if (extratime < 1000 / cl_maxfps->value)
			return;            // framerate is too high
	}

	// let the mouse activate or deactivate
	id_in->Frame();

	// decide the simulation time
	cls.frametime = extratime / 1000.0;
	cl.time += extratime;
	cls.realtime = curtime;

	extratime = 0;
#if 0
	if (cls.frametime > (1.0 / cl_minfps->value))
		cls.frametime = (1.0 / cl_minfps->value);
#else
	if (cls.frametime > (1.0 / 5))
		cls.frametime = (1.0 / 5);
#endif

	// if in the debugger last frame, don't timeout
	if (msec > 5000)
		cls.netchan.last_received = Sys_Milliseconds();

	// fetch results from server
	ReadPackets();

	// send a new command message to the server
	SendCommand();

	// predict all unacknowledged movements
	CL_PredictMovement();

	// allow rendering DLL change
	VID_CheckChanges();

	if (!cl.refresh_prepped && cls.state == ca_active)
		CL_PrepRefresh();

	// update the screen
	if (host_speeds->value)
		time_before_ref = Sys_Milliseconds();

	// Because we want to draw ImGui, we inject a callback (to a static method)
	// TODO: Instead need to have a "draw ImGui" stage of the screen proc
	// Or instead begin the ImGui frame decoupled from the actual ref!
	SCR_UpdateScreen(StaticDrawImGui);

	if (host_speeds->value)
	{
		time_after_ref = Sys_Milliseconds();

		// Move everything to the right
		int last = time_after_ref - time_before_ref;
		for (int i = 0; i < MAX_DATA_COUNT; i++) {
			int prior = timings_ref[i];

			timings_ref[i] = last;

			last = prior;
		}
	}

	// update audio
	S_Update(cl.refdef.vieworg, cl.v_forward, cl.v_right, cl.v_up);

	CDAudio_Update();

	// advance local effects for next frame
	CL_RunDLights();
	CL_RunLightStyles();
	SCR_RunCinematic();
	SCR_RunConsole();

	cls.framecount++;

	if (log_stats->value)
	{
		if (cls.state == ca_active)
		{
			if (!lasttimecalled)
			{
				lasttimecalled = Sys_Milliseconds();
				if (log_stats_file)
					fprintf(log_stats_file, "0\n");
			} else
			{
				int now = Sys_Milliseconds();

				if (log_stats_file)
					fprintf(log_stats_file, "%d\n", now - lasttimecalled);

				lasttimecalled = now;
			}
		}
	}

	if (host_speeds->value)
	{
		time_end = Sys_Milliseconds();

		// Move everything to the right
		int last = time_end - time_begin;
		for (int i = 0; i < MAX_DATA_COUNT; i++) {
			int prior = timings_cl_frame[i];

			timings_cl_frame[i] = last;

			last = prior;
		}
	}
}

//============================================================================

usercmd_t idClient::CreateCmd()
{
	usercmd_t	cmd;

	frame_msec = sys_frame_time - old_sys_frame_time;
	if (frame_msec < 1)
		frame_msec = 1;
	if (frame_msec > 200)
		frame_msec = 200;

	// get basic movement from keyboard
	CL_BaseMove (&cmd);

	// allow mice or other external controllers to add to the move
	id_in->Move(&cmd);

	CL_FinishMove (&cmd);

	old_sys_frame_time = sys_frame_time;

//cmd.impulse = cls.framecount;

	return cmd;
}

//============================================================================

// ==============
//  Client State
// ==============
void idClient::InitLocal()
{
	cls.state = ca_disconnected;
	cls.realtime = Sys_Milliseconds();

	CL_InitInput();

	adr0 = Cvar_Get("adr0", "", CVAR_ARCHIVE);
	adr1 = Cvar_Get("adr1", "", CVAR_ARCHIVE);
	adr2 = Cvar_Get("adr2", "", CVAR_ARCHIVE);
	adr3 = Cvar_Get("adr3", "", CVAR_ARCHIVE);
	adr4 = Cvar_Get("adr4", "", CVAR_ARCHIVE);
	adr5 = Cvar_Get("adr5", "", CVAR_ARCHIVE);
	adr6 = Cvar_Get("adr6", "", CVAR_ARCHIVE);
	adr7 = Cvar_Get("adr7", "", CVAR_ARCHIVE);
	adr8 = Cvar_Get("adr8", "", CVAR_ARCHIVE);

//
// register our variables
//
	cl_stereo_separation = Cvar_Get("cl_stereo_separation", "0.4", CVAR_ARCHIVE);
	cl_stereo = Cvar_Get("cl_stereo", "0", 0);

	cl_add_blend = Cvar_Get("cl_blend", "1", 0);
	cl_add_lights = Cvar_Get("cl_lights", "1", 0);
	cl_add_particles = Cvar_Get("cl_particles", "1", 0);
	cl_add_entities = Cvar_Get("cl_entities", "1", 0);
	cl_gun = Cvar_Get("cl_gun", "1", 0);
	cl_footsteps = Cvar_Get("cl_footsteps", "1", 0);
	cl_noskins = Cvar_Get("cl_noskins", "0", 0);
	cl_autoskins = Cvar_Get("cl_autoskins", "0", 0);
	cl_predict = Cvar_Get("cl_predict", "1", 0);
//	cl_minfps = Cvar_Get ("cl_minfps", "5", 0);
	cl_maxfps = Cvar_Get("cl_maxfps", "90", 0);

	cl_upspeed = Cvar_Get("cl_upspeed", "200", 0);
	cl_forwardspeed = Cvar_Get("cl_forwardspeed", "200", 0);
	cl_sidespeed = Cvar_Get("cl_sidespeed", "200", 0);
	cl_yawspeed = Cvar_Get("cl_yawspeed", "140", 0);
	cl_pitchspeed = Cvar_Get("cl_pitchspeed", "150", 0);
	cl_anglespeedkey = Cvar_Get("cl_anglespeedkey", "1.5", 0);

	cl_run = Cvar_Get("cl_run", "0", CVAR_ARCHIVE);
	freelook = Cvar_Get("freelook", "0", CVAR_ARCHIVE);
	lookspring = Cvar_Get("lookspring", "0", CVAR_ARCHIVE);
	lookstrafe = Cvar_Get("lookstrafe", "0", CVAR_ARCHIVE);
	sensitivity = Cvar_Get("sensitivity", "3", CVAR_ARCHIVE);

	m_pitch = Cvar_Get("m_pitch", "0.022", CVAR_ARCHIVE);
	m_yaw = Cvar_Get("m_yaw", "0.022", 0);
	m_forward = Cvar_Get("m_forward", "1", 0);
	m_side = Cvar_Get("m_side", "1", 0);

	cl_shownet = Cvar_Get("cl_shownet", "0", 0);
	cl_showmiss = Cvar_Get("cl_showmiss", "0", 0);
	cl_showclamp = Cvar_Get("showclamp", "0", 0);
	cl_timeout = Cvar_Get("cl_timeout", "120", 0);
	cl_paused = Cvar_Get("paused", "0", 0);
	cl_timedemo = Cvar_Get("timedemo", "0", 0);

	rcon_client_password = Cvar_Get("rcon_password", "", 0);
	rcon_address = Cvar_Get("rcon_address", "", 0);

	cl_lightlevel = Cvar_Get("r_lightlevel", "0", 0);

	//
	// userinfo
	//
	info_password = Cvar_Get("password", "", CVAR_USERINFO);
	info_spectator = Cvar_Get("spectator", "0", CVAR_USERINFO);
	name = Cvar_Get("name", "unnamed", CVAR_USERINFO | CVAR_ARCHIVE);
	skin = Cvar_Get("skin", "male/grunt", CVAR_USERINFO | CVAR_ARCHIVE);
	rate = Cvar_Get("rate", "25000", CVAR_USERINFO | CVAR_ARCHIVE);    // FIXME
	msg = Cvar_Get("msg", "1", CVAR_USERINFO | CVAR_ARCHIVE);
	hand = Cvar_Get("hand", "0", CVAR_USERINFO | CVAR_ARCHIVE);
	fov = Cvar_Get("fov", "90", CVAR_USERINFO | CVAR_ARCHIVE);
	v_fov = Cvar_Get("v_fov", "90", CVAR_USERINFO | CVAR_ARCHIVE);
	gender = Cvar_Get("gender", "male", CVAR_USERINFO | CVAR_ARCHIVE);
	gender_auto = Cvar_Get("gender_auto", "1", CVAR_ARCHIVE);
	gender->modified = false; // clear this so we know when user sets it manually

	cl_vwep = Cvar_Get("cl_vwep", "1", CVAR_ARCHIVE);

#ifdef USE_IMGUI
	imgui_visible = Cvar_Get("imgui_visible", "1", 0);
	imgui_showdemo = Cvar_Get("imgui_showdemo", "0", 0);
#endif


	//
	// register our commands
	//
	Cmd_AddCommand("cmd", CL_ForwardToServer_f);
	Cmd_AddCommand("pause", CL_Pause_f);
	Cmd_AddCommand("pingservers", CL_PingServers_f);
	Cmd_AddCommand("skins", CL_Skins_f);

	Cmd_AddCommand("userinfo", CL_Userinfo_f);
	Cmd_AddCommand("snd_restart", CL_Snd_Restart_f);

	Cmd_AddCommand("changing", CL_Changing_f);
	Cmd_AddCommand("disconnect", CL_Disconnect_f);
	Cmd_AddCommand("record", CL_Record_f);
	Cmd_AddCommand("stop", CL_Stop_f);

	Cmd_AddCommand("quit", CL_Quit_f);

	Cmd_AddCommand("connect", CL_Connect_f);
	Cmd_AddCommand("reconnect", CL_Reconnect_f);

	Cmd_AddCommand("rcon", CL_Rcon_f);

// 	Cmd_AddCommand ("packet", CL_Packet_f); // this is dangerous to leave in

	Cmd_AddCommand("setenv", CL_Setenv_f);

	Cmd_AddCommand("precache", CL_Precache_f);

	Cmd_AddCommand("download", CL_Download_f);

	//
	// forward to server commands
	//
	// the only thing this does is allow command completion
	// to work -- all unknown commands are automatically
	// forwarded to the server
	Cmd_AddCommand("wave", NULL);
	Cmd_AddCommand("inven", NULL);
	Cmd_AddCommand("kill", NULL);
	Cmd_AddCommand("use", NULL);
	Cmd_AddCommand("drop", NULL);
	Cmd_AddCommand("say", NULL);
	Cmd_AddCommand("say_team", NULL);
	Cmd_AddCommand("info", NULL);
	Cmd_AddCommand("prog", NULL);
	Cmd_AddCommand("give", NULL);
	Cmd_AddCommand("god", NULL);
	Cmd_AddCommand("notarget", NULL);
	Cmd_AddCommand("noclip", NULL);
	Cmd_AddCommand("invuse", NULL);
	Cmd_AddCommand("invprev", NULL);
	Cmd_AddCommand("invnext", NULL);
	Cmd_AddCommand("invdrop", NULL);
	Cmd_AddCommand("weapnext", NULL);
	Cmd_AddCommand("weapprev", NULL);
}

// ============
//  Networking
// ============
void idClient::ReadPackets()
{
	while (NET_GetPacket(NS_CLIENT, &net_from, &net_message))
	{
		//
		// remote command packet
		//
		if (*(int *) net_message.data == -1)
		{
			CL_ConnectionlessPacket();
			continue;
		}

		if (cls.state == ca_disconnected || cls.state == ca_connecting)
			continue;        // dump it if not connected

		if (net_message.cursize < 8)
		{
			Com_Printf("%s: Runt packet\n", NET_AdrToString(net_from));
			continue;
		}

		//
		// packet from server
		//
		if (!NET_CompareAdr(net_from, cls.netchan.remote_address))
		{
			Com_DPrintf("%s:sequenced packet without connection\n", NET_AdrToString(net_from));
			continue;
		}
		if (!Netchan_Process(&cls.netchan, &net_message))
			continue;        // wasn't accepted for some reason
		CL_ParseServerMessage();
	}

	//
	// check timeout
	//
	if (cls.state >= ca_connected && cls.realtime - cls.netchan.last_received > cl_timeout->value * 1000)
	{
		if (++cl.timeoutcount > 5)    // timeoutcount saves debugger
		{
			Com_Printf("\nServer connection timed out.\n");
			CL_Disconnect();
			return;
		}
	} else
		cl.timeoutcount = 0;
}

void idClient::SendCommand()
{
	// get new key events
	Sys_SendKeyEvents();

	// allow mice or other external controllers to add commands
	id_in->Commands();

	// process console commands
	Cbuf_Execute();

	// fix any cheating cvars
	CL_FixCvarCheats();

	// send intentions now
	SendCmd();

	// resend a connection request if necessary
	CL_CheckForResend();
}

void idClient::SendCmd()
{
	sizebuf_t	buf;
	byte		data[128];
	int			i;
	usercmd_t	*cmd, *oldcmd;
	usercmd_t	nullcmd;
	int			checksumIndex;

	// zCubed: Fixes VCRT complaining
	memset(&buf, 0, sizeof(buf));

	// build a command even if not connected

	// save this command off for prediction
	i = cls.netchan.outgoing_sequence & (CMD_BACKUP-1);
	cmd = &cl.cmds[i];
	cl.cmd_time[i] = cls.realtime;	// for netgraph ping calculation

	*cmd = CreateCmd();

	cl.cmd = *cmd;

	if (cls.state == ca_disconnected || cls.state == ca_connecting)
		return;

	if ( cls.state == ca_connected)
	{
		if (cls.netchan.message.cursize	|| curtime - cls.netchan.last_sent > 1000 )
			Netchan_Transmit (&cls.netchan, 0, buf.data);
		return;
	}

	// send a userinfo update if needed
	if (userinfo_modified)
	{
		CL_FixUpGender();
		userinfo_modified = false;
		MSG_WriteByte (&cls.netchan.message, clc_userinfo);
		MSG_WriteString (&cls.netchan.message, Cvar_Userinfo() );
	}

	SZ_Init (&buf, data, sizeof(data));

	if (cmd->buttons && cl.cinematictime > 0 && !cl.attractloop
		&& cls.realtime - cl.cinematictime > 1000)
	{	// skip the rest of the cinematic
		SCR_FinishCinematic ();
	}

	// begin a client move command
	MSG_WriteByte (&buf, clc_move);

	// save the position for a checksum byte
	checksumIndex = buf.cursize;
	MSG_WriteByte (&buf, 0);

	// let the server know what the last frame we
	// got was, so the next message can be delta compressed
	if (cl_nodelta->value || !cl.frame.valid || cls.demowaiting)
		MSG_WriteLong (&buf, -1);	// no compression
	else
		MSG_WriteLong (&buf, cl.frame.serverframe);

	// send this and the previous cmds in the message, so
	// if the last packet was dropped, it can be recovered
	i = (cls.netchan.outgoing_sequence-2) & (CMD_BACKUP-1);
	cmd = &cl.cmds[i];
	memset (&nullcmd, 0, sizeof(nullcmd));
	MSG_WriteDeltaUsercmd (&buf, &nullcmd, cmd);
	oldcmd = cmd;

	i = (cls.netchan.outgoing_sequence-1) & (CMD_BACKUP-1);
	cmd = &cl.cmds[i];
	MSG_WriteDeltaUsercmd (&buf, oldcmd, cmd);
	oldcmd = cmd;

	i = (cls.netchan.outgoing_sequence) & (CMD_BACKUP-1);
	cmd = &cl.cmds[i];
	MSG_WriteDeltaUsercmd (&buf, oldcmd, cmd);

	// calculate a checksum over the move commands
	buf.data[checksumIndex] = COM_BlockSequenceCRCByte(
		buf.data + checksumIndex + 1, buf.cursize - checksumIndex - 1,
		cls.netchan.outgoing_sequence);

	//
	// deliver the message
	//
	Netchan_Transmit (&cls.netchan, buf.cursize, buf.data);
}

//============================================================================

// =============
//  Client Cmds
// =============
void idClient::CCmdDemoRecord()
{
	char	name[MAX_OSPATH];
	char	buf_data[MAX_MSGLEN];
	sizebuf_t	buf;
	int		i;
	int		len;
	entity_state_t	*ent;
	entity_state_t	nullstate;

	if (Cmd_Argc() != 2)
	{
		Com_Printf ("record <demoname>\n");
		return;
	}

	if (cls.demorecording)
	{
		Com_Printf ("Already recording.\n");
		return;
	}

	if (cls.state != ca_active)
	{
		Com_Printf ("You must be in a level to record.\n");
		return;
	}

	//
	// open the demo file
	//
	Com_sprintf (name, sizeof(name), "%s/demos/%s.dm2", FS_Gamedir(), Cmd_Argv(1));

	Com_Printf ("recording to %s.\n", name);
	FS_CreatePath (name);
	cls.demofile = fopen (name, "wb");
	if (!cls.demofile)
	{
		Com_Printf ("ERROR: couldn't open.\n");
		return;
	}
	cls.demorecording = true;

	// don't start saving messages until a non-delta compressed message is received
	cls.demowaiting = true;

	//
	// write out messages to hold the startup information
	//
	SZ_Init (&buf, reinterpret_cast<byte*>(buf_data), sizeof(buf_data));

	// send the serverdata
	MSG_WriteByte (&buf, svc_serverdata);
	MSG_WriteLong (&buf, PROTOCOL_VERSION);
	MSG_WriteLong (&buf, 0x10000 + cl.servercount);
	MSG_WriteByte (&buf, 1);	// demos are always attract loops
	MSG_WriteString (&buf, cl.gamedir);
	MSG_WriteShort (&buf, cl.playernum);

	MSG_WriteString (&buf, cl.configstrings[CS_NAME]);

	// configstrings
	for (i=0 ; i<MAX_CONFIGSTRINGS ; i++)
	{
		if (cl.configstrings[i][0])
		{
			if (buf.cursize + strlen (cl.configstrings[i]) + 32 > buf.maxsize)
			{	// write it out
				len = LittleLong (buf.cursize);
				fwrite (&len, 4, 1, cls.demofile);
				fwrite (buf.data, buf.cursize, 1, cls.demofile);
				buf.cursize = 0;
			}

			MSG_WriteByte (&buf, svc_configstring);
			MSG_WriteShort (&buf, i);
			MSG_WriteString (&buf, cl.configstrings[i]);
		}

	}

	// baselines
	memset (&nullstate, 0, sizeof(nullstate));
	for (i=0; i<MAX_EDICTS ; i++)
	{
		ent = &cl_entities[i].baseline;
		if (!ent->modelindex)
			continue;

		if (buf.cursize + 64 > buf.maxsize)
		{	// write it out
			len = LittleLong (buf.cursize);
			fwrite (&len, 4, 1, cls.demofile);
			fwrite (buf.data, buf.cursize, 1, cls.demofile);
			buf.cursize = 0;
		}

		MSG_WriteByte (&buf, svc_spawnbaseline);
		MSG_WriteDeltaEntity (&nullstate, &cl_entities[i].baseline, &buf, true, true);
	}

	MSG_WriteByte (&buf, svc_stufftext);
	MSG_WriteString (&buf, "precache\n");

	// write it to the demo file

	len = LittleLong (buf.cursize);
	fwrite (&len, 4, 1, cls.demofile);
	fwrite (buf.data, buf.cursize, 1, cls.demofile);

	// the rest of the demo file will be individual frames
}

//============================================================================

void idClient::StaticDrawImGui()
{
#ifdef USE_IMGUI
	if (!imgui_visible->value)
		return;

	ImGui::Begin("idClient");

	// Last timings
	if (ImGui::TreeNode("Timings"))
	{
		if (ImGui::Button("toggle host_speeds"))
		{
			if (host_speeds->value)
				Cvar_Set("host_speeds", "0");
			else
				Cvar_Set("host_speeds", "1");
		}

		ImGui::PlotLines("idClient::RunFrame()", timings_cl_frame, MAX_DATA_COUNT);
		ImGui::PlotLines("idClient::RunFrame()->SCR_UpdateScreen", timings_ref, MAX_DATA_COUNT);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Cvars"))
	{
		if (ImGui::BeginTable("Cvar Table", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			cvar_t *var;

			ImGui::TableSetupColumn("Name");
			ImGui::TableSetupColumn("String Value");
			ImGui::TableSetupColumn("Float Value");
			ImGui::TableHeadersRow();

			for (var = cvar_vars; var; var = var->next)
			{
				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", var->name);

				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", var->string);

				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%f", var->value);
			}

			ImGui::EndTable();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Client State"))
	{
		if (ImGui::TreeNode("Current Frame"))
		{
			ImGui::Text("deltaframe: %i", cl.frame.deltaframe);

			if (ImGui::TreeNode("Player State"))
			{
				ImGui::Text("fov: %f", cl.frame.playerstate.fov);
				ImGui::Text("v_fov: %f", cl.frame.playerstate.v_fov);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Movement (Decoded)"))
		{
			pmove_state_t state = cl.frame.playerstate.pmove;
			vec3_t origin, velocity;

			origin[0] = state.origin[0] * 0.125;
			origin[1] = state.origin[1] * 0.125;
			origin[2] = state.origin[2] * 0.125;

			velocity[0] = state.velocity[0] * 0.125;
			velocity[1] = state.velocity[1] * 0.125;
			velocity[2] = state.velocity[2] * 0.125;

			ImGui::Text("origin: %f, %f, %f", origin[0], origin[1], origin[2]);
			ImGui::Text("velocity: %f, %f, %f", velocity[0], velocity[1], velocity[2]);
			ImGui::Text("magnitude: %f", VectorNormalize(velocity));

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}

	ImGui::End();

	if (imgui_showdemo->value)
		ImGui::ShowDemoWindow();
#endif
}

//============================================================================
