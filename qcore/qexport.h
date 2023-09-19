/*
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
// qexport.h - Defines .dll / .so visibility attributes
//

#ifndef ZEALOT_QEXPORT_H
#define ZEALOT_QEXPORT_H

#ifndef __cplusplus

#ifdef WIN32
#define QEXPORT __declspec(dllexport)
#endif

#if defined(__clang__) || defined(__GNUC__)
#define QEXPORT __attribute__((visibility("default")))
#endif

#ifndef QEXPORT
#define QEXPORT
#endif

#define QEXTERN_FUNC
#define QEXTERN_VAR extern

#else

#ifdef WIN32
#define QEXPORT __declspec(dllexport) extern "C"
#endif

#ifndef QEXPORT
#define QEXPORT extern "C"
#endif

#define QEXTERN_FUNC extern
#define QEXTERN_VAR extern

#endif

#endif //ZEALOT_QEXPORT_H
