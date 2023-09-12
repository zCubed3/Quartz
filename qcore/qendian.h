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
// qendian.h - Common byte manipulation functions (with respect to endianness)
//

#ifndef ZEALOT_QENDIAN_H
#define ZEALOT_QENDIAN_H

//============================================================================

#include "qtypes.h"

//============================================================================

extern qboolean		big_endian;

//============================================================================

short	BigShort(short l);
short	LittleShort(short l);
int		BigLong(int l);
int		LittleLong(int l);
float	BigFloat(float l);
float	LittleFloat(float l);

void	Swap_Init(void);

//============================================================================

#endif//ZEALOT_QENDIAN_H
