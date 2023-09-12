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
// qmath.h - Shared math types / functions
//

#ifndef ZEALOT_QMATH_H
#define ZEALOT_QMATH_H

#include "qtypes.h"

//============================================================================
//
// Vector Math
//
//============================================================================

typedef float vec_t;

typedef vec_t vec3_t[3];
typedef vec_t vec5_t[5];

struct cplane_s;

//============================================================================

extern vec3_t vec3_origin;

//============================================================================

#define DotProduct(x,y)			(x[0]*y[0]+x[1]*y[1]+x[2]*y[2])
#define VectorSubtract(a,b,c)	(c[0]=a[0]-b[0],c[1]=a[1]-b[1],c[2]=a[2]-b[2])
#define VectorAdd(a,b,c)		(c[0]=a[0]+b[0],c[1]=a[1]+b[1],c[2]=a[2]+b[2])
#define VectorCopy(a,b)			(b[0]=a[0],b[1]=a[1],b[2]=a[2])
#define VectorClear(a)			(a[0]=a[1]=a[2]=0)
#define VectorNegate(a,b)		(b[0]=-a[0],b[1]=-a[1],b[2]=-a[2])
#define VectorSet(v, x, y, z)	(v[0]=(x), v[1]=(y), v[2]=(z))

//============================================================================

vec_t 	_DotProduct(vec3_t v1, vec3_t v2);
void 	_VectorSubtract(vec3_t veca, vec3_t vecb, vec3_t out);
void 	_VectorAdd(vec3_t veca, vec3_t vecb, vec3_t out);
void 	_VectorCopy(vec3_t in, vec3_t out);

//============================================================================

void 	ClearBounds(vec3_t mins, vec3_t maxs);
void 	AddPointToBounds(vec3_t v, vec3_t mins, vec3_t maxs);

int 	VectorCompare(vec3_t v1, vec3_t v2);
vec_t 	VectorLength(vec3_t v);

void 	CrossProduct(vec3_t v1, vec3_t v2, vec3_t cross);

void 	VectorMA (vec3_t veca, float scale, vec3_t vecb, vec3_t vecc);

vec_t 	VectorNormalize(vec3_t v);		// returns vector length
vec_t 	VectorNormalize2(vec3_t v, vec3_t out);

void 	VectorInverse(vec3_t v);

void 	VectorScale(vec3_t in, vec_t scale, vec3_t out);

void 	AngleVectors(vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);

int 	BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, struct cplane_s *plane);

void 	ProjectPointOnPlane( vec3_t dst, const vec3_t p, const vec3_t normal );

void 	PerpendicularVector( vec3_t dst, const vec3_t src );

void 	RotatePointAroundVector( vec3_t dst, const vec3_t dir, const vec3_t point, float degrees );

//============================================================================

#define DEG2RAD( a ) (( a * M_PI ) / 180.0F)

#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

//============================================================================

void R_ConcatRotations (float in1[3][3], float in2[3][3], float out[3][3]);
void R_ConcatTransforms (float in1[3][4], float in2[3][4], float out[3][4]);

//============================================================================

//============================================================================
//
// Floating point math
//
//============================================================================

typedef	int	fixed4_t;
typedef	int	fixed8_t;
typedef	int	fixed16_t;

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

//============================================================================

//long Q_ftol( float f );
#define Q_ftol(f) ((long)f);

int 	Q_log2(int val);

//============================================================================

float 	LerpAngle (float a1, float a2, float frac);

float	anglemod(float a);

//============================================================================

#define	nanmask (255<<23)

#define	IS_NAN(x) (((*(int *)&x)&nanmask)==nanmask)

//============================================================================

typedef struct cplane_s
{
	vec3_t	normal;
	float	dist;
	byte	type;			// for fast side tests
	byte	signbits;		// signx + (signy<<1) + (signz<<1)
	byte	pad[2];
} cplane_t;

//============================================================================

#endif//ZEALOT_QMATH_H
