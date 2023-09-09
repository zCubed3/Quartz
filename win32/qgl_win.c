/*
Copyright (C) 1997-2001 Id Software, Inc.

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
/*
** QGL_WIN.C
**
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake2 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/
#include <float.h>
#include "../ref_gl/gl_local.h"
#include "glw_win.h"

int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
int   ( WINAPI * qwglGetPixelFormat)(HDC);
BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
HGLRC ( WINAPI * qwglCreateContext)(HDC);
HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

void ( APIENTRY * glAccum )(GLenum op, GLfloat value);
void ( APIENTRY * glAlphaFunc )(GLenum func, GLclampf ref);
GLboolean ( APIENTRY * glAreTexturesResident )(GLsizei n, const GLuint *textures, GLboolean *residences);
void ( APIENTRY * glArrayElement )(GLint i);
void ( APIENTRY * glBegin )(GLenum mode);
void ( APIENTRY * glBindTexture )(GLenum target, GLuint texture);
void ( APIENTRY * glBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
void ( APIENTRY * glBlendFunc )(GLenum sfactor, GLenum dfactor);
void ( APIENTRY * glCallList )(GLuint list);
void ( APIENTRY * glCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
void ( APIENTRY * glClear )(GLbitfield mask);
void ( APIENTRY * glClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ( APIENTRY * glClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void ( APIENTRY * glClearDepth )(GLclampd depth);
void ( APIENTRY * glClearIndex )(GLfloat c);
void ( APIENTRY * glClearStencil )(GLint s);
void ( APIENTRY * glClipPlane )(GLenum plane, const GLdouble *equation);
void ( APIENTRY * glColor3b )(GLbyte red, GLbyte green, GLbyte blue);
void ( APIENTRY * glColor3bv )(const GLbyte *v);
void ( APIENTRY * glColor3d )(GLdouble red, GLdouble green, GLdouble blue);
void ( APIENTRY * glColor3dv )(const GLdouble *v);
void ( APIENTRY * glColor3f )(GLfloat red, GLfloat green, GLfloat blue);
void ( APIENTRY * glColor3fv )(const GLfloat *v);
void ( APIENTRY * glColor3i )(GLint red, GLint green, GLint blue);
void ( APIENTRY * glColor3iv )(const GLint *v);
void ( APIENTRY * glColor3s )(GLshort red, GLshort green, GLshort blue);
void ( APIENTRY * glColor3sv )(const GLshort *v);
void ( APIENTRY * glColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
void ( APIENTRY * glColor3ubv )(const GLubyte *v);
void ( APIENTRY * glColor3ui )(GLuint red, GLuint green, GLuint blue);
void ( APIENTRY * glColor3uiv )(const GLuint *v);
void ( APIENTRY * glColor3us )(GLushort red, GLushort green, GLushort blue);
void ( APIENTRY * glColor3usv )(const GLushort *v);
void ( APIENTRY * glColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
void ( APIENTRY * glColor4bv )(const GLbyte *v);
void ( APIENTRY * glColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
void ( APIENTRY * glColor4dv )(const GLdouble *v);
void ( APIENTRY * glColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ( APIENTRY * glColor4fv )(const GLfloat *v);
void ( APIENTRY * glColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
void ( APIENTRY * glColor4iv )(const GLint *v);
void ( APIENTRY * glColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
void ( APIENTRY * glColor4sv )(const GLshort *v);
void ( APIENTRY * glColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
void ( APIENTRY * glColor4ubv )(const GLubyte *v);
void ( APIENTRY * glColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
void ( APIENTRY * glColor4uiv )(const GLuint *v);
void ( APIENTRY * glColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
void ( APIENTRY * glColor4usv )(const GLushort *v);
void ( APIENTRY * glColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void ( APIENTRY * glColorMaterial )(GLenum face, GLenum mode);
void ( APIENTRY * glColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
void ( APIENTRY * glCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
void ( APIENTRY * glCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void ( APIENTRY * glCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void ( APIENTRY * glCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY * glCullFace )(GLenum mode);
void ( APIENTRY * glDeleteLists )(GLuint list, GLsizei range);
void ( APIENTRY * glDeleteTextures )(GLsizei n, const GLuint *textures);
void ( APIENTRY * glDepthFunc )(GLenum func);
void ( APIENTRY * glDepthMask )(GLboolean flag);
void ( APIENTRY * glDepthRange )(GLclampd zNear, GLclampd zFar);
void ( APIENTRY * glDisable )(GLenum cap);
void ( APIENTRY * glDisableClientState )(GLenum array);
void ( APIENTRY * glDrawArrays )(GLenum mode, GLint first, GLsizei count);
void ( APIENTRY * glDrawBuffer )(GLenum mode);
void ( APIENTRY * glDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void ( APIENTRY * glDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * glEdgeFlag )(GLboolean flag);
void ( APIENTRY * glEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glEdgeFlagv )(const GLboolean *flag);
void ( APIENTRY * glEnable )(GLenum cap);
void ( APIENTRY * glEnableClientState )(GLenum array);
void ( APIENTRY * glEnd )(void);
void ( APIENTRY * glEndList )(void);
void ( APIENTRY * glEvalCoord1d )(GLdouble u);
void ( APIENTRY * glEvalCoord1dv )(const GLdouble *u);
void ( APIENTRY * glEvalCoord1f )(GLfloat u);
void ( APIENTRY * glEvalCoord1fv )(const GLfloat *u);
void ( APIENTRY * glEvalCoord2d )(GLdouble u, GLdouble v);
void ( APIENTRY * glEvalCoord2dv )(const GLdouble *u);
void ( APIENTRY * glEvalCoord2f )(GLfloat u, GLfloat v);
void ( APIENTRY * glEvalCoord2fv )(const GLfloat *u);
void ( APIENTRY * glEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
void ( APIENTRY * glEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
void ( APIENTRY * glEvalPoint1 )(GLint i);
void ( APIENTRY * glEvalPoint2 )(GLint i, GLint j);
void ( APIENTRY * glFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
void ( APIENTRY * glFinish )(void);
void ( APIENTRY * glFlush )(void);
void ( APIENTRY * glFogf )(GLenum pname, GLfloat param);
void ( APIENTRY * glFogfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY * glFogi )(GLenum pname, GLint param);
void ( APIENTRY * glFogiv )(GLenum pname, const GLint *params);
void ( APIENTRY * glFrontFace )(GLenum mode);
void ( APIENTRY * glFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLuint ( APIENTRY * glGenLists )(GLsizei range);
void ( APIENTRY * glGenTextures )(GLsizei n, GLuint *textures);
void ( APIENTRY * glGetBooleanv )(GLenum pname, GLboolean *params);
void ( APIENTRY * glGetClipPlane )(GLenum plane, GLdouble *equation);
void ( APIENTRY * glGetDoublev )(GLenum pname, GLdouble *params);
GLenum ( APIENTRY * glGetError )(void);
void ( APIENTRY * glGetFloatv )(GLenum pname, GLfloat *params);
void ( APIENTRY * glGetIntegerv )(GLenum pname, GLint *params);
void ( APIENTRY * glGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetLightiv )(GLenum light, GLenum pname, GLint *params);
void ( APIENTRY * glGetMapdv )(GLenum target, GLenum query, GLdouble *v);
void ( APIENTRY * glGetMapfv )(GLenum target, GLenum query, GLfloat *v);
void ( APIENTRY * glGetMapiv )(GLenum target, GLenum query, GLint *v);
void ( APIENTRY * glGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
void ( APIENTRY * glGetPixelMapfv )(GLenum map, GLfloat *values);
void ( APIENTRY * glGetPixelMapuiv )(GLenum map, GLuint *values);
void ( APIENTRY * glGetPixelMapusv )(GLenum map, GLushort *values);
void ( APIENTRY * glGetPointerv )(GLenum pname, GLvoid* *params);
void ( APIENTRY * glGetPolygonStipple )(GLubyte *mask);
const GLubyte * ( APIENTRY * glGetString )(GLenum name);
void ( APIENTRY * glGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY * glGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
void ( APIENTRY * glGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
void ( APIENTRY * glGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
void ( APIENTRY * glGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
void ( APIENTRY * glGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
void ( APIENTRY * glGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
void ( APIENTRY * glHint )(GLenum target, GLenum mode);
void ( APIENTRY * glIndexMask )(GLuint mask);
void ( APIENTRY * glIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glIndexd )(GLdouble c);
void ( APIENTRY * glIndexdv )(const GLdouble *c);
void ( APIENTRY * glIndexf )(GLfloat c);
void ( APIENTRY * glIndexfv )(const GLfloat *c);
void ( APIENTRY * glIndexi )(GLint c);
void ( APIENTRY * glIndexiv )(const GLint *c);
void ( APIENTRY * glIndexs )(GLshort c);
void ( APIENTRY * glIndexsv )(const GLshort *c);
void ( APIENTRY * glIndexub )(GLubyte c);
void ( APIENTRY * glIndexubv )(const GLubyte *c);
void ( APIENTRY * glInitNames )(void);
void ( APIENTRY * glInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean ( APIENTRY * glIsEnabled )(GLenum cap);
GLboolean ( APIENTRY * glIsList )(GLuint list);
GLboolean ( APIENTRY * glIsTexture )(GLuint texture);
void ( APIENTRY * glLightModelf )(GLenum pname, GLfloat param);
void ( APIENTRY * glLightModelfv )(GLenum pname, const GLfloat *params);
void ( APIENTRY * glLightModeli )(GLenum pname, GLint param);
void ( APIENTRY * glLightModeliv )(GLenum pname, const GLint *params);
void ( APIENTRY * glLightf )(GLenum light, GLenum pname, GLfloat param);
void ( APIENTRY * glLightfv )(GLenum light, GLenum pname, const GLfloat *params);
void ( APIENTRY * glLighti )(GLenum light, GLenum pname, GLint param);
void ( APIENTRY * glLightiv )(GLenum light, GLenum pname, const GLint *params);
void ( APIENTRY * glLineStipple )(GLint factor, GLushort pattern);
void ( APIENTRY * glLineWidth )(GLfloat width);
void ( APIENTRY * glListBase )(GLuint base);
void ( APIENTRY * glLoadIdentity )(void);
void ( APIENTRY * glLoadMatrixd )(const GLdouble *m);
void ( APIENTRY * glLoadMatrixf )(const GLfloat *m);
void ( APIENTRY * glLoadName )(GLuint name);
void ( APIENTRY * glLogicOp )(GLenum opcode);
void ( APIENTRY * glMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
void ( APIENTRY * glMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
void ( APIENTRY * glMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
void ( APIENTRY * glMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
void ( APIENTRY * glMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
void ( APIENTRY * glMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
void ( APIENTRY * glMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
void ( APIENTRY * glMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
void ( APIENTRY * glMaterialf )(GLenum face, GLenum pname, GLfloat param);
void ( APIENTRY * glMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
void ( APIENTRY * glMateriali )(GLenum face, GLenum pname, GLint param);
void ( APIENTRY * glMaterialiv )(GLenum face, GLenum pname, const GLint *params);
void ( APIENTRY * glMatrixMode )(GLenum mode);
void ( APIENTRY * glMultMatrixd )(const GLdouble *m);
void ( APIENTRY * glMultMatrixf )(const GLfloat *m);
void ( APIENTRY * glNewList )(GLuint list, GLenum mode);
void ( APIENTRY * glNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
void ( APIENTRY * glNormal3bv )(const GLbyte *v);
void ( APIENTRY * glNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
void ( APIENTRY * glNormal3dv )(const GLdouble *v);
void ( APIENTRY * glNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
void ( APIENTRY * glNormal3fv )(const GLfloat *v);
void ( APIENTRY * glNormal3i )(GLint nx, GLint ny, GLint nz);
void ( APIENTRY * glNormal3iv )(const GLint *v);
void ( APIENTRY * glNormal3s )(GLshort nx, GLshort ny, GLshort nz);
void ( APIENTRY * glNormal3sv )(const GLshort *v);
void ( APIENTRY * glNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void ( APIENTRY * glPassThrough )(GLfloat token);
void ( APIENTRY * glPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
void ( APIENTRY * glPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
void ( APIENTRY * glPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
void ( APIENTRY * glPixelStoref )(GLenum pname, GLfloat param);
void ( APIENTRY * glPixelStorei )(GLenum pname, GLint param);
void ( APIENTRY * glPixelTransferf )(GLenum pname, GLfloat param);
void ( APIENTRY * glPixelTransferi )(GLenum pname, GLint param);
void ( APIENTRY * glPixelZoom )(GLfloat xfactor, GLfloat yfactor);
void ( APIENTRY * glPointSize )(GLfloat size);
void ( APIENTRY * glPolygonMode )(GLenum face, GLenum mode);
void ( APIENTRY * glPolygonOffset )(GLfloat factor, GLfloat units);
void ( APIENTRY * glPolygonStipple )(const GLubyte *mask);
void ( APIENTRY * glPopAttrib )(void);
void ( APIENTRY * glPopClientAttrib )(void);
void ( APIENTRY * glPopMatrix )(void);
void ( APIENTRY * glPopName )(void);
void ( APIENTRY * glPrioritizeTextures )(GLsizei n, const GLuint *textures, const GLclampf *priorities);
void ( APIENTRY * glPushAttrib )(GLbitfield mask);
void ( APIENTRY * glPushClientAttrib )(GLbitfield mask);
void ( APIENTRY * glPushMatrix )(void);
void ( APIENTRY * glPushName )(GLuint name);
void ( APIENTRY * glRasterPos2d )(GLdouble x, GLdouble y);
void ( APIENTRY * glRasterPos2dv )(const GLdouble *v);
void ( APIENTRY * glRasterPos2f )(GLfloat x, GLfloat y);
void ( APIENTRY * glRasterPos2fv )(const GLfloat *v);
void ( APIENTRY * glRasterPos2i )(GLint x, GLint y);
void ( APIENTRY * glRasterPos2iv )(const GLint *v);
void ( APIENTRY * glRasterPos2s )(GLshort x, GLshort y);
void ( APIENTRY * glRasterPos2sv )(const GLshort *v);
void ( APIENTRY * glRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY * glRasterPos3dv )(const GLdouble *v);
void ( APIENTRY * glRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * glRasterPos3fv )(const GLfloat *v);
void ( APIENTRY * glRasterPos3i )(GLint x, GLint y, GLint z);
void ( APIENTRY * glRasterPos3iv )(const GLint *v);
void ( APIENTRY * glRasterPos3s )(GLshort x, GLshort y, GLshort z);
void ( APIENTRY * glRasterPos3sv )(const GLshort *v);
void ( APIENTRY * glRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void ( APIENTRY * glRasterPos4dv )(const GLdouble *v);
void ( APIENTRY * glRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY * glRasterPos4fv )(const GLfloat *v);
void ( APIENTRY * glRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
void ( APIENTRY * glRasterPos4iv )(const GLint *v);
void ( APIENTRY * glRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
void ( APIENTRY * glRasterPos4sv )(const GLshort *v);
void ( APIENTRY * glReadBuffer )(GLenum mode);
void ( APIENTRY * glReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
void ( APIENTRY * glRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
void ( APIENTRY * glRectdv )(const GLdouble *v1, const GLdouble *v2);
void ( APIENTRY * glRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
void ( APIENTRY * glRectfv )(const GLfloat *v1, const GLfloat *v2);
void ( APIENTRY * glRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
void ( APIENTRY * glRectiv )(const GLint *v1, const GLint *v2);
void ( APIENTRY * glRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
void ( APIENTRY * glRectsv )(const GLshort *v1, const GLshort *v2);
GLint ( APIENTRY * glRenderMode )(GLenum mode);
void ( APIENTRY * glRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY * glRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * glScaled )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY * glScalef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * glScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
void ( APIENTRY * glSelectBuffer )(GLsizei size, GLuint *buffer);
void ( APIENTRY * glShadeModel )(GLenum mode);
void ( APIENTRY * glStencilFunc )(GLenum func, GLint ref, GLuint mask);
void ( APIENTRY * glStencilMask )(GLuint mask);
void ( APIENTRY * glStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
void ( APIENTRY * glTexCoord1d )(GLdouble s);
void ( APIENTRY * glTexCoord1dv )(const GLdouble *v);
void ( APIENTRY * glTexCoord1f )(GLfloat s);
void ( APIENTRY * glTexCoord1fv )(const GLfloat *v);
void ( APIENTRY * glTexCoord1i )(GLint s);
void ( APIENTRY * glTexCoord1iv )(const GLint *v);
void ( APIENTRY * glTexCoord1s )(GLshort s);
void ( APIENTRY * glTexCoord1sv )(const GLshort *v);
void ( APIENTRY * glTexCoord2d )(GLdouble s, GLdouble t);
void ( APIENTRY * glTexCoord2dv )(const GLdouble *v);
void ( APIENTRY * glTexCoord2f )(GLfloat s, GLfloat t);
void ( APIENTRY * glTexCoord2fv )(const GLfloat *v);
void ( APIENTRY * glTexCoord2i )(GLint s, GLint t);
void ( APIENTRY * glTexCoord2iv )(const GLint *v);
void ( APIENTRY * glTexCoord2s )(GLshort s, GLshort t);
void ( APIENTRY * glTexCoord2sv )(const GLshort *v);
void ( APIENTRY * glTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
void ( APIENTRY * glTexCoord3dv )(const GLdouble *v);
void ( APIENTRY * glTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
void ( APIENTRY * glTexCoord3fv )(const GLfloat *v);
void ( APIENTRY * glTexCoord3i )(GLint s, GLint t, GLint r);
void ( APIENTRY * glTexCoord3iv )(const GLint *v);
void ( APIENTRY * glTexCoord3s )(GLshort s, GLshort t, GLshort r);
void ( APIENTRY * glTexCoord3sv )(const GLshort *v);
void ( APIENTRY * glTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
void ( APIENTRY * glTexCoord4dv )(const GLdouble *v);
void ( APIENTRY * glTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void ( APIENTRY * glTexCoord4fv )(const GLfloat *v);
void ( APIENTRY * glTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
void ( APIENTRY * glTexCoord4iv )(const GLint *v);
void ( APIENTRY * glTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
void ( APIENTRY * glTexCoord4sv )(const GLshort *v);
void ( APIENTRY * glTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glTexEnvf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY * glTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY * glTexEnvi )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY * glTexEnviv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY * glTexGend )(GLenum coord, GLenum pname, GLdouble param);
void ( APIENTRY * glTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
void ( APIENTRY * glTexGenf )(GLenum coord, GLenum pname, GLfloat param);
void ( APIENTRY * glTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
void ( APIENTRY * glTexGeni )(GLenum coord, GLenum pname, GLint param);
void ( APIENTRY * glTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
void ( APIENTRY * glTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * glTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * glTexParameterf )(GLenum target, GLenum pname, GLfloat param);
void ( APIENTRY * glTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
void ( APIENTRY * glTexParameteri )(GLenum target, GLenum pname, GLint param);
void ( APIENTRY * glTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
void ( APIENTRY * glTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * glTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
void ( APIENTRY * glTranslated )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY * glTranslatef )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * glVertex2d )(GLdouble x, GLdouble y);
void ( APIENTRY * glVertex2dv )(const GLdouble *v);
void ( APIENTRY * glVertex2f )(GLfloat x, GLfloat y);
void ( APIENTRY * glVertex2fv )(const GLfloat *v);
void ( APIENTRY * glVertex2i )(GLint x, GLint y);
void ( APIENTRY * glVertex2iv )(const GLint *v);
void ( APIENTRY * glVertex2s )(GLshort x, GLshort y);
void ( APIENTRY * glVertex2sv )(const GLshort *v);
void ( APIENTRY * glVertex3d )(GLdouble x, GLdouble y, GLdouble z);
void ( APIENTRY * glVertex3dv )(const GLdouble *v);
void ( APIENTRY * glVertex3f )(GLfloat x, GLfloat y, GLfloat z);
void ( APIENTRY * glVertex3fv )(const GLfloat *v);
void ( APIENTRY * glVertex3i )(GLint x, GLint y, GLint z);
void ( APIENTRY * glVertex3iv )(const GLint *v);
void ( APIENTRY * glVertex3s )(GLshort x, GLshort y, GLshort z);
void ( APIENTRY * glVertex3sv )(const GLshort *v);
void ( APIENTRY * glVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void ( APIENTRY * glVertex4dv )(const GLdouble *v);
void ( APIENTRY * glVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void ( APIENTRY * glVertex4fv )(const GLfloat *v);
void ( APIENTRY * glVertex4i )(GLint x, GLint y, GLint z, GLint w);
void ( APIENTRY * glVertex4iv )(const GLint *v);
void ( APIENTRY * glVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
void ( APIENTRY * glVertex4sv )(const GLshort *v);
void ( APIENTRY * glVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
void ( APIENTRY * glViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

void ( APIENTRY * glLockArraysEXT)( int, int);
void ( APIENTRY * glUnlockArraysEXT) ( void );

BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );
BOOL ( WINAPI * qwglGetDeviceGammaRampEXT)( unsigned char *, unsigned char *, unsigned char * );
BOOL ( WINAPI * qwglSetDeviceGammaRampEXT)( const unsigned char *, const unsigned char *, const unsigned char * );
void ( APIENTRY * glPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY * glPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY * glColorTableEXT)( int, int, int, int, int, const void * );
void ( APIENTRY * glSelectTextureSGIS)( GLenum );
void ( APIENTRY * glMultiTexCoord2f)( GLenum, GLfloat, GLfloat );
void ( APIENTRY * glActiveTextureARB) ( GLenum );
void ( APIENTRY * glClientActiveTextureARB) ( GLenum );

static void ( APIENTRY * dllAccum )(GLenum op, GLfloat value);
static void ( APIENTRY * dllAlphaFunc )(GLenum func, GLclampf ref);
GLboolean ( APIENTRY * dllAreTexturesResident )(GLsizei n, const GLuint *textures, GLboolean *residences);
static void ( APIENTRY * dllArrayElement )(GLint i);
static void ( APIENTRY * dllBegin )(GLenum mode);
static void ( APIENTRY * dllBindTexture )(GLenum target, GLuint texture);
static void ( APIENTRY * dllBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
static void ( APIENTRY * dllBlendFunc )(GLenum sfactor, GLenum dfactor);
static void ( APIENTRY * dllCallList )(GLuint list);
static void ( APIENTRY * dllCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
static void ( APIENTRY * dllClear )(GLbitfield mask);
static void ( APIENTRY * dllClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static void ( APIENTRY * dllClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
static void ( APIENTRY * dllClearDepth )(GLclampd depth);
static void ( APIENTRY * dllClearIndex )(GLfloat c);
static void ( APIENTRY * dllClearStencil )(GLint s);
static void ( APIENTRY * dllClipPlane )(GLenum plane, const GLdouble *equation);
static void ( APIENTRY * dllColor3b )(GLbyte red, GLbyte green, GLbyte blue);
static void ( APIENTRY * dllColor3bv )(const GLbyte *v);
static void ( APIENTRY * dllColor3d )(GLdouble red, GLdouble green, GLdouble blue);
static void ( APIENTRY * dllColor3dv )(const GLdouble *v);
static void ( APIENTRY * dllColor3f )(GLfloat red, GLfloat green, GLfloat blue);
static void ( APIENTRY * dllColor3fv )(const GLfloat *v);
static void ( APIENTRY * dllColor3i )(GLint red, GLint green, GLint blue);
static void ( APIENTRY * dllColor3iv )(const GLint *v);
static void ( APIENTRY * dllColor3s )(GLshort red, GLshort green, GLshort blue);
static void ( APIENTRY * dllColor3sv )(const GLshort *v);
static void ( APIENTRY * dllColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
static void ( APIENTRY * dllColor3ubv )(const GLubyte *v);
static void ( APIENTRY * dllColor3ui )(GLuint red, GLuint green, GLuint blue);
static void ( APIENTRY * dllColor3uiv )(const GLuint *v);
static void ( APIENTRY * dllColor3us )(GLushort red, GLushort green, GLushort blue);
static void ( APIENTRY * dllColor3usv )(const GLushort *v);
static void ( APIENTRY * dllColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
static void ( APIENTRY * dllColor4bv )(const GLbyte *v);
static void ( APIENTRY * dllColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
static void ( APIENTRY * dllColor4dv )(const GLdouble *v);
static void ( APIENTRY * dllColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static void ( APIENTRY * dllColor4fv )(const GLfloat *v);
static void ( APIENTRY * dllColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
static void ( APIENTRY * dllColor4iv )(const GLint *v);
static void ( APIENTRY * dllColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
static void ( APIENTRY * dllColor4sv )(const GLshort *v);
static void ( APIENTRY * dllColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
static void ( APIENTRY * dllColor4ubv )(const GLubyte *v);
static void ( APIENTRY * dllColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
static void ( APIENTRY * dllColor4uiv )(const GLuint *v);
static void ( APIENTRY * dllColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
static void ( APIENTRY * dllColor4usv )(const GLushort *v);
static void ( APIENTRY * dllColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
static void ( APIENTRY * dllColorMaterial )(GLenum face, GLenum mode);
static void ( APIENTRY * dllColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
static void ( APIENTRY * dllCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
static void ( APIENTRY * dllCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
static void ( APIENTRY * dllCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
static void ( APIENTRY * dllCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
static void ( APIENTRY * dllCullFace )(GLenum mode);
static void ( APIENTRY * dllDeleteLists )(GLuint list, GLsizei range);
static void ( APIENTRY * dllDeleteTextures )(GLsizei n, const GLuint *textures);
static void ( APIENTRY * dllDepthFunc )(GLenum func);
static void ( APIENTRY * dllDepthMask )(GLboolean flag);
static void ( APIENTRY * dllDepthRange )(GLclampd zNear, GLclampd zFar);
static void ( APIENTRY * dllDisable )(GLenum cap);
static void ( APIENTRY * dllDisableClientState )(GLenum array);
static void ( APIENTRY * dllDrawArrays )(GLenum mode, GLint first, GLsizei count);
static void ( APIENTRY * dllDrawBuffer )(GLenum mode);
static void ( APIENTRY * dllDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
static void ( APIENTRY * dllDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllEdgeFlag )(GLboolean flag);
static void ( APIENTRY * dllEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllEdgeFlagv )(const GLboolean *flag);
static void ( APIENTRY * dllEnable )(GLenum cap);
static void ( APIENTRY * dllEnableClientState )(GLenum array);
static void ( APIENTRY * dllEnd )(void);
static void ( APIENTRY * dllEndList )(void);
static void ( APIENTRY * dllEvalCoord1d )(GLdouble u);
static void ( APIENTRY * dllEvalCoord1dv )(const GLdouble *u);
static void ( APIENTRY * dllEvalCoord1f )(GLfloat u);
static void ( APIENTRY * dllEvalCoord1fv )(const GLfloat *u);
static void ( APIENTRY * dllEvalCoord2d )(GLdouble u, GLdouble v);
static void ( APIENTRY * dllEvalCoord2dv )(const GLdouble *u);
static void ( APIENTRY * dllEvalCoord2f )(GLfloat u, GLfloat v);
static void ( APIENTRY * dllEvalCoord2fv )(const GLfloat *u);
static void ( APIENTRY * dllEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
static void ( APIENTRY * dllEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
static void ( APIENTRY * dllEvalPoint1 )(GLint i);
static void ( APIENTRY * dllEvalPoint2 )(GLint i, GLint j);
static void ( APIENTRY * dllFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
static void ( APIENTRY * dllFinish )(void);
static void ( APIENTRY * dllFlush )(void);
static void ( APIENTRY * dllFogf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllFogfv )(GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllFogi )(GLenum pname, GLint param);
static void ( APIENTRY * dllFogiv )(GLenum pname, const GLint *params);
static void ( APIENTRY * dllFrontFace )(GLenum mode);
static void ( APIENTRY * dllFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLuint ( APIENTRY * dllGenLists )(GLsizei range);
static void ( APIENTRY * dllGenTextures )(GLsizei n, GLuint *textures);
static void ( APIENTRY * dllGetBooleanv )(GLenum pname, GLboolean *params);
static void ( APIENTRY * dllGetClipPlane )(GLenum plane, GLdouble *equation);
static void ( APIENTRY * dllGetDoublev )(GLenum pname, GLdouble *params);
GLenum ( APIENTRY * dllGetError )(void);
static void ( APIENTRY * dllGetFloatv )(GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetIntegerv )(GLenum pname, GLint *params);
static void ( APIENTRY * dllGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetLightiv )(GLenum light, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetMapdv )(GLenum target, GLenum query, GLdouble *v);
static void ( APIENTRY * dllGetMapfv )(GLenum target, GLenum query, GLfloat *v);
static void ( APIENTRY * dllGetMapiv )(GLenum target, GLenum query, GLint *v);
static void ( APIENTRY * dllGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetPixelMapfv )(GLenum map, GLfloat *values);
static void ( APIENTRY * dllGetPixelMapuiv )(GLenum map, GLuint *values);
static void ( APIENTRY * dllGetPixelMapusv )(GLenum map, GLushort *values);
static void ( APIENTRY * dllGetPointerv )(GLenum pname, GLvoid* *params);
static void ( APIENTRY * dllGetPolygonStipple )(GLubyte *mask);
const GLubyte * ( APIENTRY * dllGetString )(GLenum name);
static void ( APIENTRY * dllGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
static void ( APIENTRY * dllGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
static void ( APIENTRY * dllGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
static void ( APIENTRY * dllGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
static void ( APIENTRY * dllGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
static void ( APIENTRY * dllHint )(GLenum target, GLenum mode);
static void ( APIENTRY * dllIndexMask )(GLuint mask);
static void ( APIENTRY * dllIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllIndexd )(GLdouble c);
static void ( APIENTRY * dllIndexdv )(const GLdouble *c);
static void ( APIENTRY * dllIndexf )(GLfloat c);
static void ( APIENTRY * dllIndexfv )(const GLfloat *c);
static void ( APIENTRY * dllIndexi )(GLint c);
static void ( APIENTRY * dllIndexiv )(const GLint *c);
static void ( APIENTRY * dllIndexs )(GLshort c);
static void ( APIENTRY * dllIndexsv )(const GLshort *c);
static void ( APIENTRY * dllIndexub )(GLubyte c);
static void ( APIENTRY * dllIndexubv )(const GLubyte *c);
static void ( APIENTRY * dllInitNames )(void);
static void ( APIENTRY * dllInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
GLboolean ( APIENTRY * dllIsEnabled )(GLenum cap);
GLboolean ( APIENTRY * dllIsList )(GLuint list);
GLboolean ( APIENTRY * dllIsTexture )(GLuint texture);
static void ( APIENTRY * dllLightModelf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllLightModelfv )(GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllLightModeli )(GLenum pname, GLint param);
static void ( APIENTRY * dllLightModeliv )(GLenum pname, const GLint *params);
static void ( APIENTRY * dllLightf )(GLenum light, GLenum pname, GLfloat param);
static void ( APIENTRY * dllLightfv )(GLenum light, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllLighti )(GLenum light, GLenum pname, GLint param);
static void ( APIENTRY * dllLightiv )(GLenum light, GLenum pname, const GLint *params);
static void ( APIENTRY * dllLineStipple )(GLint factor, GLushort pattern);
static void ( APIENTRY * dllLineWidth )(GLfloat width);
static void ( APIENTRY * dllListBase )(GLuint base);
static void ( APIENTRY * dllLoadIdentity )(void);
static void ( APIENTRY * dllLoadMatrixd )(const GLdouble *m);
static void ( APIENTRY * dllLoadMatrixf )(const GLfloat *m);
static void ( APIENTRY * dllLoadName )(GLuint name);
static void ( APIENTRY * dllLogicOp )(GLenum opcode);
static void ( APIENTRY * dllMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
static void ( APIENTRY * dllMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
static void ( APIENTRY * dllMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
static void ( APIENTRY * dllMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
static void ( APIENTRY * dllMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
static void ( APIENTRY * dllMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
static void ( APIENTRY * dllMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
static void ( APIENTRY * dllMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
static void ( APIENTRY * dllMaterialf )(GLenum face, GLenum pname, GLfloat param);
static void ( APIENTRY * dllMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllMateriali )(GLenum face, GLenum pname, GLint param);
static void ( APIENTRY * dllMaterialiv )(GLenum face, GLenum pname, const GLint *params);
static void ( APIENTRY * dllMatrixMode )(GLenum mode);
static void ( APIENTRY * dllMultMatrixd )(const GLdouble *m);
static void ( APIENTRY * dllMultMatrixf )(const GLfloat *m);
static void ( APIENTRY * dllNewList )(GLuint list, GLenum mode);
static void ( APIENTRY * dllNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
static void ( APIENTRY * dllNormal3bv )(const GLbyte *v);
static void ( APIENTRY * dllNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
static void ( APIENTRY * dllNormal3dv )(const GLdouble *v);
static void ( APIENTRY * dllNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
static void ( APIENTRY * dllNormal3fv )(const GLfloat *v);
static void ( APIENTRY * dllNormal3i )(GLint nx, GLint ny, GLint nz);
static void ( APIENTRY * dllNormal3iv )(const GLint *v);
static void ( APIENTRY * dllNormal3s )(GLshort nx, GLshort ny, GLshort nz);
static void ( APIENTRY * dllNormal3sv )(const GLshort *v);
static void ( APIENTRY * dllNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
static void ( APIENTRY * dllPassThrough )(GLfloat token);
static void ( APIENTRY * dllPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
static void ( APIENTRY * dllPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
static void ( APIENTRY * dllPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
static void ( APIENTRY * dllPixelStoref )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllPixelStorei )(GLenum pname, GLint param);
static void ( APIENTRY * dllPixelTransferf )(GLenum pname, GLfloat param);
static void ( APIENTRY * dllPixelTransferi )(GLenum pname, GLint param);
static void ( APIENTRY * dllPixelZoom )(GLfloat xfactor, GLfloat yfactor);
static void ( APIENTRY * dllPointSize )(GLfloat size);
static void ( APIENTRY * dllPolygonMode )(GLenum face, GLenum mode);
static void ( APIENTRY * dllPolygonOffset )(GLfloat factor, GLfloat units);
static void ( APIENTRY * dllPolygonStipple )(const GLubyte *mask);
static void ( APIENTRY * dllPopAttrib )(void);
static void ( APIENTRY * dllPopClientAttrib )(void);
static void ( APIENTRY * dllPopMatrix )(void);
static void ( APIENTRY * dllPopName )(void);
static void ( APIENTRY * dllPrioritizeTextures )(GLsizei n, const GLuint *textures, const GLclampf *priorities);
static void ( APIENTRY * dllPushAttrib )(GLbitfield mask);
static void ( APIENTRY * dllPushClientAttrib )(GLbitfield mask);
static void ( APIENTRY * dllPushMatrix )(void);
static void ( APIENTRY * dllPushName )(GLuint name);
static void ( APIENTRY * dllRasterPos2d )(GLdouble x, GLdouble y);
static void ( APIENTRY * dllRasterPos2dv )(const GLdouble *v);
static void ( APIENTRY * dllRasterPos2f )(GLfloat x, GLfloat y);
static void ( APIENTRY * dllRasterPos2fv )(const GLfloat *v);
static void ( APIENTRY * dllRasterPos2i )(GLint x, GLint y);
static void ( APIENTRY * dllRasterPos2iv )(const GLint *v);
static void ( APIENTRY * dllRasterPos2s )(GLshort x, GLshort y);
static void ( APIENTRY * dllRasterPos2sv )(const GLshort *v);
static void ( APIENTRY * dllRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
static void ( APIENTRY * dllRasterPos3dv )(const GLdouble *v);
static void ( APIENTRY * dllRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllRasterPos3fv )(const GLfloat *v);
static void ( APIENTRY * dllRasterPos3i )(GLint x, GLint y, GLint z);
static void ( APIENTRY * dllRasterPos3iv )(const GLint *v);
static void ( APIENTRY * dllRasterPos3s )(GLshort x, GLshort y, GLshort z);
static void ( APIENTRY * dllRasterPos3sv )(const GLshort *v);
static void ( APIENTRY * dllRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
static void ( APIENTRY * dllRasterPos4dv )(const GLdouble *v);
static void ( APIENTRY * dllRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
static void ( APIENTRY * dllRasterPos4fv )(const GLfloat *v);
static void ( APIENTRY * dllRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
static void ( APIENTRY * dllRasterPos4iv )(const GLint *v);
static void ( APIENTRY * dllRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
static void ( APIENTRY * dllRasterPos4sv )(const GLshort *v);
static void ( APIENTRY * dllReadBuffer )(GLenum mode);
static void ( APIENTRY * dllReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
static void ( APIENTRY * dllRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
static void ( APIENTRY * dllRectdv )(const GLdouble *v1, const GLdouble *v2);
static void ( APIENTRY * dllRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
static void ( APIENTRY * dllRectfv )(const GLfloat *v1, const GLfloat *v2);
static void ( APIENTRY * dllRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
static void ( APIENTRY * dllRectiv )(const GLint *v1, const GLint *v2);
static void ( APIENTRY * dllRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
static void ( APIENTRY * dllRectsv )(const GLshort *v1, const GLshort *v2);
GLint ( APIENTRY * dllRenderMode )(GLenum mode);
static void ( APIENTRY * dllRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
static void ( APIENTRY * dllRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllScaled )(GLdouble x, GLdouble y, GLdouble z);
static void ( APIENTRY * dllScalef )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
static void ( APIENTRY * dllSelectBuffer )(GLsizei size, GLuint *buffer);
static void ( APIENTRY * dllShadeModel )(GLenum mode);
static void ( APIENTRY * dllStencilFunc )(GLenum func, GLint ref, GLuint mask);
static void ( APIENTRY * dllStencilMask )(GLuint mask);
static void ( APIENTRY * dllStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
static void ( APIENTRY * dllTexCoord1d )(GLdouble s);
static void ( APIENTRY * dllTexCoord1dv )(const GLdouble *v);
static void ( APIENTRY * dllTexCoord1f )(GLfloat s);
static void ( APIENTRY * dllTexCoord1fv )(const GLfloat *v);
static void ( APIENTRY * dllTexCoord1i )(GLint s);
static void ( APIENTRY * dllTexCoord1iv )(const GLint *v);
static void ( APIENTRY * dllTexCoord1s )(GLshort s);
static void ( APIENTRY * dllTexCoord1sv )(const GLshort *v);
static void ( APIENTRY * dllTexCoord2d )(GLdouble s, GLdouble t);
static void ( APIENTRY * dllTexCoord2dv )(const GLdouble *v);
static void ( APIENTRY * dllTexCoord2f )(GLfloat s, GLfloat t);
static void ( APIENTRY * dllTexCoord2fv )(const GLfloat *v);
static void ( APIENTRY * dllTexCoord2i )(GLint s, GLint t);
static void ( APIENTRY * dllTexCoord2iv )(const GLint *v);
static void ( APIENTRY * dllTexCoord2s )(GLshort s, GLshort t);
static void ( APIENTRY * dllTexCoord2sv )(const GLshort *v);
static void ( APIENTRY * dllTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
static void ( APIENTRY * dllTexCoord3dv )(const GLdouble *v);
static void ( APIENTRY * dllTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
static void ( APIENTRY * dllTexCoord3fv )(const GLfloat *v);
static void ( APIENTRY * dllTexCoord3i )(GLint s, GLint t, GLint r);
static void ( APIENTRY * dllTexCoord3iv )(const GLint *v);
static void ( APIENTRY * dllTexCoord3s )(GLshort s, GLshort t, GLshort r);
static void ( APIENTRY * dllTexCoord3sv )(const GLshort *v);
static void ( APIENTRY * dllTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
static void ( APIENTRY * dllTexCoord4dv )(const GLdouble *v);
static void ( APIENTRY * dllTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
static void ( APIENTRY * dllTexCoord4fv )(const GLfloat *v);
static void ( APIENTRY * dllTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
static void ( APIENTRY * dllTexCoord4iv )(const GLint *v);
static void ( APIENTRY * dllTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
static void ( APIENTRY * dllTexCoord4sv )(const GLshort *v);
static void ( APIENTRY * dllTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllTexEnvf )(GLenum target, GLenum pname, GLfloat param);
static void ( APIENTRY * dllTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllTexEnvi )(GLenum target, GLenum pname, GLint param);
static void ( APIENTRY * dllTexEnviv )(GLenum target, GLenum pname, const GLint *params);
static void ( APIENTRY * dllTexGend )(GLenum coord, GLenum pname, GLdouble param);
static void ( APIENTRY * dllTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
static void ( APIENTRY * dllTexGenf )(GLenum coord, GLenum pname, GLfloat param);
static void ( APIENTRY * dllTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllTexGeni )(GLenum coord, GLenum pname, GLint param);
static void ( APIENTRY * dllTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
static void ( APIENTRY * dllTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTexParameterf )(GLenum target, GLenum pname, GLfloat param);
static void ( APIENTRY * dllTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
static void ( APIENTRY * dllTexParameteri )(GLenum target, GLenum pname, GLint param);
static void ( APIENTRY * dllTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
static void ( APIENTRY * dllTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
static void ( APIENTRY * dllTranslated )(GLdouble x, GLdouble y, GLdouble z);
static void ( APIENTRY * dllTranslatef )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllVertex2d )(GLdouble x, GLdouble y);
static void ( APIENTRY * dllVertex2dv )(const GLdouble *v);
static void ( APIENTRY * dllVertex2f )(GLfloat x, GLfloat y);
static void ( APIENTRY * dllVertex2fv )(const GLfloat *v);
static void ( APIENTRY * dllVertex2i )(GLint x, GLint y);
static void ( APIENTRY * dllVertex2iv )(const GLint *v);
static void ( APIENTRY * dllVertex2s )(GLshort x, GLshort y);
static void ( APIENTRY * dllVertex2sv )(const GLshort *v);
static void ( APIENTRY * dllVertex3d )(GLdouble x, GLdouble y, GLdouble z);
static void ( APIENTRY * dllVertex3dv )(const GLdouble *v);
static void ( APIENTRY * dllVertex3f )(GLfloat x, GLfloat y, GLfloat z);
static void ( APIENTRY * dllVertex3fv )(const GLfloat *v);
static void ( APIENTRY * dllVertex3i )(GLint x, GLint y, GLint z);
static void ( APIENTRY * dllVertex3iv )(const GLint *v);
static void ( APIENTRY * dllVertex3s )(GLshort x, GLshort y, GLshort z);
static void ( APIENTRY * dllVertex3sv )(const GLshort *v);
static void ( APIENTRY * dllVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
static void ( APIENTRY * dllVertex4dv )(const GLdouble *v);
static void ( APIENTRY * dllVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
static void ( APIENTRY * dllVertex4fv )(const GLfloat *v);
static void ( APIENTRY * dllVertex4i )(GLint x, GLint y, GLint z, GLint w);
static void ( APIENTRY * dllVertex4iv )(const GLint *v);
static void ( APIENTRY * dllVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
static void ( APIENTRY * dllVertex4sv )(const GLshort *v);
static void ( APIENTRY * dllVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
static void ( APIENTRY * dllViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

static void APIENTRY logAccum(GLenum op, GLfloat value)
{
	fprintf( glw_state.log_fp, "glAccum\n" );
	dllAccum( op, value );
}

static void APIENTRY logAlphaFunc(GLenum func, GLclampf ref)
{
	fprintf( glw_state.log_fp, "glAlphaFunc( 0x%x, %f )\n", func, ref );
	dllAlphaFunc( func, ref );
}

static GLboolean APIENTRY logAreTexturesResident(GLsizei n, const GLuint *textures, GLboolean *residences)
{
	fprintf( glw_state.log_fp, "glAreTexturesResident\n" );
	return dllAreTexturesResident( n, textures, residences );
}

static void APIENTRY logArrayElement(GLint i)
{
	fprintf( glw_state.log_fp, "glArrayElement\n" );
	dllArrayElement( i );
}

static void APIENTRY logBegin(GLenum mode)
{
	fprintf( glw_state.log_fp, "glBegin( 0x%x )\n", mode );
	dllBegin( mode );
}

static void APIENTRY logBindTexture(GLenum target, GLuint texture)
{
	fprintf( glw_state.log_fp, "glBindTexture( 0x%x, %u )\n", target, texture );
	dllBindTexture( target, texture );
}

static void APIENTRY logBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap)
{
	fprintf( glw_state.log_fp, "glBitmap\n" );
	dllBitmap( width, height, xorig, yorig, xmove, ymove, bitmap );
}

static void APIENTRY logBlendFunc(GLenum sfactor, GLenum dfactor)
{
	fprintf( glw_state.log_fp, "glBlendFunc( 0x%x, 0x%x )\n", sfactor, dfactor );
	dllBlendFunc( sfactor, dfactor );
}

static void APIENTRY logCallList(GLuint list)
{
	fprintf( glw_state.log_fp, "glCallList( %u )\n", list );
	dllCallList( list );
}

static void APIENTRY logCallLists(GLsizei n, GLenum type, const void *lists)
{
	fprintf( glw_state.log_fp, "glCallLists\n" );
	dllCallLists( n, type, lists );
}

static void APIENTRY logClear(GLbitfield mask)
{
	fprintf( glw_state.log_fp, "glClear\n" );
	dllClear( mask );
}

static void APIENTRY logClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	fprintf( glw_state.log_fp, "glClearAccum\n" );
	dllClearAccum( red, green, blue, alpha );
}

static void APIENTRY logClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	fprintf( glw_state.log_fp, "glClearColor\n" );
	dllClearColor( red, green, blue, alpha );
}

static void APIENTRY logClearDepth(GLclampd depth)
{
	fprintf( glw_state.log_fp, "glClearDepth\n" );
	dllClearDepth( depth );
}

static void APIENTRY logClearIndex(GLfloat c)
{
	fprintf( glw_state.log_fp, "glClearIndex\n" );
	dllClearIndex( c );
}

static void APIENTRY logClearStencil(GLint s)
{
	fprintf( glw_state.log_fp, "glClearStencil\n" );
	dllClearStencil( s );
}

static void APIENTRY logClipPlane(GLenum plane, const GLdouble *equation)
{
	fprintf( glw_state.log_fp, "glClipPlane\n" );
	dllClipPlane( plane, equation );
}

static void APIENTRY logColor3b(GLbyte red, GLbyte green, GLbyte blue)
{
	fprintf( glw_state.log_fp, "glColor3b\n" );
	dllColor3b( red, green, blue );
}

static void APIENTRY logColor3bv(const GLbyte *v)
{
	fprintf( glw_state.log_fp, "glColor3bv\n" );
	dllColor3bv( v );
}

static void APIENTRY logColor3d(GLdouble red, GLdouble green, GLdouble blue)
{
	fprintf( glw_state.log_fp, "glColor3d\n" );
	dllColor3d( red, green, blue );
}

static void APIENTRY logColor3dv(const GLdouble *v)
{
	fprintf( glw_state.log_fp, "glColor3dv\n" );
	dllColor3dv( v );
}

static void APIENTRY logColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
	fprintf( glw_state.log_fp, "glColor3f\n" );
	dllColor3f( red, green, blue );
}

static void APIENTRY logColor3fv(const GLfloat *v)
{
	fprintf( glw_state.log_fp, "glColor3fv\n" );
	dllColor3fv( v );
}

static void APIENTRY logColor3i(GLint red, GLint green, GLint blue)
{
	fprintf( glw_state.log_fp, "glColor3i\n" );
	dllColor3i( red, green, blue );
}

static void APIENTRY logColor3iv(const GLint *v)
{
	fprintf( glw_state.log_fp, "glColor3iv\n" );
	dllColor3iv( v );
}

static void APIENTRY logColor3s(GLshort red, GLshort green, GLshort blue)
{
	fprintf( glw_state.log_fp, "glColor3s\n" );
	dllColor3s( red, green, blue );
}

static void APIENTRY logColor3sv(const GLshort *v)
{
	fprintf( glw_state.log_fp, "glColor3sv\n" );
	dllColor3sv( v );
}

static void APIENTRY logColor3ub(GLubyte red, GLubyte green, GLubyte blue)
{
	fprintf( glw_state.log_fp, "glColor3ub\n" );
	dllColor3ub( red, green, blue );
}

static void APIENTRY logColor3ubv(const GLubyte *v)
{
	fprintf( glw_state.log_fp, "glColor3ubv\n" );
	dllColor3ubv( v );
}

#define SIG( x ) fprintf( glw_state.log_fp, x "\n" )

static void APIENTRY logColor3ui(GLuint red, GLuint green, GLuint blue)
{
	SIG( "glColor3ui" );
	dllColor3ui( red, green, blue );
}

static void APIENTRY logColor3uiv(const GLuint *v)
{
	SIG( "glColor3uiv" );
	dllColor3uiv( v );
}

static void APIENTRY logColor3us(GLushort red, GLushort green, GLushort blue)
{
	SIG( "glColor3us" );
	dllColor3us( red, green, blue );
}

static void APIENTRY logColor3usv(const GLushort *v)
{
	SIG( "glColor3usv" );
	dllColor3usv( v );
}

static void APIENTRY logColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)
{
	SIG( "glColor4b" );
	dllColor4b( red, green, blue, alpha );
}

static void APIENTRY logColor4bv(const GLbyte *v)
{
	SIG( "glColor4bv" );
	dllColor4bv( v );
}

static void APIENTRY logColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
	SIG( "glColor4d" );
	dllColor4d( red, green, blue, alpha );
}
static void APIENTRY logColor4dv(const GLdouble *v)
{
	SIG( "glColor4dv" );
	dllColor4dv( v );
}
static void APIENTRY logColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	fprintf( glw_state.log_fp, "glColor4f( %f,%f,%f,%f )\n", red, green, blue, alpha );
	dllColor4f( red, green, blue, alpha );
}
static void APIENTRY logColor4fv(const GLfloat *v)
{
	fprintf( glw_state.log_fp, "glColor4fv( %f,%f,%f,%f )\n", v[0], v[1], v[2], v[3] );
	dllColor4fv( v );
}
static void APIENTRY logColor4i(GLint red, GLint green, GLint blue, GLint alpha)
{
	SIG( "glColor4i" );
	dllColor4i( red, green, blue, alpha );
}
static void APIENTRY logColor4iv(const GLint *v)
{
	SIG( "glColor4iv" );
	dllColor4iv( v );
}
static void APIENTRY logColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)
{
	SIG( "glColor4s" );
	dllColor4s( red, green, blue, alpha );
}
static void APIENTRY logColor4sv(const GLshort *v)
{
	SIG( "glColor4sv" );
	dllColor4sv( v );
}
static void APIENTRY logColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)
{
	SIG( "glColor4b" );
	dllColor4b( red, green, blue, alpha );
}
static void APIENTRY logColor4ubv(const GLubyte *v)
{
	SIG( "glColor4ubv" );
	dllColor4ubv( v );
}
static void APIENTRY logColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)
{
	SIG( "glColor4ui" );
	dllColor4ui( red, green, blue, alpha );
}
static void APIENTRY logColor4uiv(const GLuint *v)
{
	SIG( "glColor4uiv" );
	dllColor4uiv( v );
}
static void APIENTRY logColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)
{
	SIG( "glColor4us" );
	dllColor4us( red, green, blue, alpha );
}
static void APIENTRY logColor4usv(const GLushort *v)
{
	SIG( "glColor4usv" );
	dllColor4usv( v );
}
static void APIENTRY logColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	SIG( "glColorMask" );
	dllColorMask( red, green, blue, alpha );
}
static void APIENTRY logColorMaterial(GLenum face, GLenum mode)
{
	SIG( "glColorMaterial" );
	dllColorMaterial( face, mode );
}

static void APIENTRY logColorPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	SIG( "glColorPointer" );
	dllColorPointer( size, type, stride, pointer );
}

static void APIENTRY logCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)
{
	SIG( "glCopyPixels" );
	dllCopyPixels( x, y, width, height, type );
}

static void APIENTRY logCopyTexImage1D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border)
{
	SIG( "glCopyTexImage1D" );
	dllCopyTexImage1D( target, level, internalFormat, x, y, width, border );
}

static void APIENTRY logCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	SIG( "glCopyTexImage2D" );
	dllCopyTexImage2D( target, level, internalFormat, x, y, width, height, border );
}

static void APIENTRY logCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)
{
	SIG( "glCopyTexSubImage1D" );
	dllCopyTexSubImage1D( target, level, xoffset, x, y, width );
}

static void APIENTRY logCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	SIG( "glCopyTexSubImage2D" );
	dllCopyTexSubImage2D( target, level, xoffset, yoffset, x, y, width, height );
}

static void APIENTRY logCullFace(GLenum mode)
{
	SIG( "glCullFace" );
	dllCullFace( mode );
}

static void APIENTRY logDeleteLists(GLuint list, GLsizei range)
{
	SIG( "glDeleteLists" );
	dllDeleteLists( list, range );
}

static void APIENTRY logDeleteTextures(GLsizei n, const GLuint *textures)
{
	SIG( "glDeleteTextures" );
	dllDeleteTextures( n, textures );
}

static void APIENTRY logDepthFunc(GLenum func)
{
	SIG( "glDepthFunc" );
	dllDepthFunc( func );
}

static void APIENTRY logDepthMask(GLboolean flag)
{
	SIG( "glDepthMask" );
	dllDepthMask( flag );
}

static void APIENTRY logDepthRange(GLclampd zNear, GLclampd zFar)
{
	SIG( "glDepthRange" );
	dllDepthRange( zNear, zFar );
}

static void APIENTRY logDisable(GLenum cap)
{
	fprintf( glw_state.log_fp, "glDisable( 0x%x )\n", cap );
	dllDisable( cap );
}

static void APIENTRY logDisableClientState(GLenum array)
{
	SIG( "glDisableClientState" );
	dllDisableClientState( array );
}

static void APIENTRY logDrawArrays(GLenum mode, GLint first, GLsizei count)
{
	SIG( "glDrawArrays" );
	dllDrawArrays( mode, first, count );
}

static void APIENTRY logDrawBuffer(GLenum mode)
{
	SIG( "glDrawBuffer" );
	dllDrawBuffer( mode );
}

static void APIENTRY logDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices)
{
	SIG( "glDrawElements" );
	dllDrawElements( mode, count, type, indices );
}

static void APIENTRY logDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
	SIG( "glDrawPixels" );
	dllDrawPixels( width, height, format, type, pixels );
}

static void APIENTRY logEdgeFlag(GLboolean flag)
{
	SIG( "glEdgeFlag" );
	dllEdgeFlag( flag );
}

static void APIENTRY logEdgeFlagPointer(GLsizei stride, const void *pointer)
{
	SIG( "glEdgeFlagPointer" );
	dllEdgeFlagPointer( stride, pointer );
}

static void APIENTRY logEdgeFlagv(const GLboolean *flag)
{
	SIG( "glEdgeFlagv" );
	dllEdgeFlagv( flag );
}

static void APIENTRY logEnable(GLenum cap)
{
	fprintf( glw_state.log_fp, "glEnable( 0x%x )\n", cap );
	dllEnable( cap );
}

static void APIENTRY logEnableClientState(GLenum array)
{
	SIG( "glEnableClientState" );
	dllEnableClientState( array );
}

static void APIENTRY logEnd(void)
{
	SIG( "glEnd" );
	dllEnd();
}

static void APIENTRY logEndList(void)
{
	SIG( "glEndList" );
	dllEndList();
}

static void APIENTRY logEvalCoord1d(GLdouble u)
{
	SIG( "glEvalCoord1d" );
	dllEvalCoord1d( u );
}

static void APIENTRY logEvalCoord1dv(const GLdouble *u)
{
	SIG( "glEvalCoord1dv" );
	dllEvalCoord1dv( u );
}

static void APIENTRY logEvalCoord1f(GLfloat u)
{
	SIG( "glEvalCoord1f" );
	dllEvalCoord1f( u );
}

static void APIENTRY logEvalCoord1fv(const GLfloat *u)
{
	SIG( "glEvalCoord1fv" );
	dllEvalCoord1fv( u );
}
static void APIENTRY logEvalCoord2d(GLdouble u, GLdouble v)
{
	SIG( "glEvalCoord2d" );
	dllEvalCoord2d( u, v );
}
static void APIENTRY logEvalCoord2dv(const GLdouble *u)
{
	SIG( "glEvalCoord2dv" );
	dllEvalCoord2dv( u );
}
static void APIENTRY logEvalCoord2f(GLfloat u, GLfloat v)
{
	SIG( "glEvalCoord2f" );
	dllEvalCoord2f( u, v );
}
static void APIENTRY logEvalCoord2fv(const GLfloat *u)
{
	SIG( "glEvalCoord2fv" );
	dllEvalCoord2fv( u );
}

static void APIENTRY logEvalMesh1(GLenum mode, GLint i1, GLint i2)
{
	SIG( "glEvalMesh1" );
	dllEvalMesh1( mode, i1, i2 );
}
static void APIENTRY logEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)
{
	SIG( "glEvalMesh2" );
	dllEvalMesh2( mode, i1, i2, j1, j2 );
}
static void APIENTRY logEvalPoint1(GLint i)
{
	SIG( "glEvalPoint1" );
	dllEvalPoint1( i );
}
static void APIENTRY logEvalPoint2(GLint i, GLint j)
{
	SIG( "glEvalPoint2" );
	dllEvalPoint2( i, j );
}

static void APIENTRY logFeedbackBuffer(GLsizei size, GLenum type, GLfloat *buffer)
{
	SIG( "glFeedbackBuffer" );
	dllFeedbackBuffer( size, type, buffer );
}

static void APIENTRY logFinish(void)
{
	SIG( "glFinish" );
	dllFinish();
}

static void APIENTRY logFlush(void)
{
	SIG( "glFlush" );
	dllFlush();
}

static void APIENTRY logFogf(GLenum pname, GLfloat param)
{
	SIG( "glFogf" );
	dllFogf( pname, param );
}

static void APIENTRY logFogfv(GLenum pname, const GLfloat *params)
{
	SIG( "glFogfv" );
	dllFogfv( pname, params );
}

static void APIENTRY logFogi(GLenum pname, GLint param)
{
	SIG( "glFogi" );
	dllFogi( pname, param );
}

static void APIENTRY logFogiv(GLenum pname, const GLint *params)
{
	SIG( "glFogiv" );
	dllFogiv( pname, params );
}

static void APIENTRY logFrontFace(GLenum mode)
{
	SIG( "glFrontFace" );
	dllFrontFace( mode );
}

static void APIENTRY logFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	SIG( "glFrustum" );
	dllFrustum( left, right, bottom, top, zNear, zFar );
}

static GLuint APIENTRY logGenLists(GLsizei range)
{
	SIG( "glGenLists" );
	return dllGenLists( range );
}

static void APIENTRY logGenTextures(GLsizei n, GLuint *textures)
{
	SIG( "glGenTextures" );
	dllGenTextures( n, textures );
}

static void APIENTRY logGetBooleanv(GLenum pname, GLboolean *params)
{
	SIG( "glGetBooleanv" );
	dllGetBooleanv( pname, params );
}

static void APIENTRY logGetClipPlane(GLenum plane, GLdouble *equation)
{
	SIG( "glGetClipPlane" );
	dllGetClipPlane( plane, equation );
}

static void APIENTRY logGetDoublev(GLenum pname, GLdouble *params)
{
	SIG( "glGetDoublev" );
	dllGetDoublev( pname, params );
}

static GLenum APIENTRY logGetError(void)
{
	SIG( "glGetError" );
	return dllGetError();
}

static void APIENTRY logGetFloatv(GLenum pname, GLfloat *params)
{
	SIG( "glGetFloatv" );
	dllGetFloatv( pname, params );
}

static void APIENTRY logGetIntegerv(GLenum pname, GLint *params)
{
	SIG( "glGetIntegerv" );
	dllGetIntegerv( pname, params );
}

static void APIENTRY logGetLightfv(GLenum light, GLenum pname, GLfloat *params)
{
	SIG( "glGetLightfv" );
	dllGetLightfv( light, pname, params );
}

static void APIENTRY logGetLightiv(GLenum light, GLenum pname, GLint *params)
{
	SIG( "glGetLightiv" );
	dllGetLightiv( light, pname, params );
}

static void APIENTRY logGetMapdv(GLenum target, GLenum query, GLdouble *v)
{
	SIG( "glGetMapdv" );
	dllGetMapdv( target, query, v );
}

static void APIENTRY logGetMapfv(GLenum target, GLenum query, GLfloat *v)
{
	SIG( "glGetMapfv" );
	dllGetMapfv( target, query, v );
}

static void APIENTRY logGetMapiv(GLenum target, GLenum query, GLint *v)
{
	SIG( "glGetMapiv" );
	dllGetMapiv( target, query, v );
}

static void APIENTRY logGetMaterialfv(GLenum face, GLenum pname, GLfloat *params)
{
	SIG( "glGetMaterialfv" );
	dllGetMaterialfv( face, pname, params );
}

static void APIENTRY logGetMaterialiv(GLenum face, GLenum pname, GLint *params)
{
	SIG( "glGetMaterialiv" );
	dllGetMaterialiv( face, pname, params );
}

static void APIENTRY logGetPixelMapfv(GLenum map, GLfloat *values)
{
	SIG( "glGetPixelMapfv" );
	dllGetPixelMapfv( map, values );
}

static void APIENTRY logGetPixelMapuiv(GLenum map, GLuint *values)
{
	SIG( "glGetPixelMapuiv" );
	dllGetPixelMapuiv( map, values );
}

static void APIENTRY logGetPixelMapusv(GLenum map, GLushort *values)
{
	SIG( "glGetPixelMapusv" );
	dllGetPixelMapusv( map, values );
}

static void APIENTRY logGetPointerv(GLenum pname, GLvoid* *params)
{
	SIG( "glGetPointerv" );
	dllGetPointerv( pname, params );
}

static void APIENTRY logGetPolygonStipple(GLubyte *mask)
{
	SIG( "glGetPolygonStipple" );
	dllGetPolygonStipple( mask );
}

static const GLubyte * APIENTRY logGetString(GLenum name)
{
	SIG( "glGetString" );
	return dllGetString( name );
}

static void APIENTRY logGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params)
{
	SIG( "glGetTexEnvfv" );
	dllGetTexEnvfv( target, pname, params );
}

static void APIENTRY logGetTexEnviv(GLenum target, GLenum pname, GLint *params)
{
	SIG( "glGetTexEnviv" );
	dllGetTexEnviv( target, pname, params );
}

static void APIENTRY logGetTexGendv(GLenum coord, GLenum pname, GLdouble *params)
{
	SIG( "glGetTexGendv" );
	dllGetTexGendv( coord, pname, params );
}

static void APIENTRY logGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params)
{
	SIG( "glGetTexGenfv" );
	dllGetTexGenfv( coord, pname, params );
}

static void APIENTRY logGetTexGeniv(GLenum coord, GLenum pname, GLint *params)
{
	SIG( "glGetTexGeniv" );
	dllGetTexGeniv( coord, pname, params );
}

static void APIENTRY logGetTexImage(GLenum target, GLint level, GLenum format, GLenum type, void *pixels)
{
	SIG( "glGetTexImage" );
	dllGetTexImage( target, level, format, type, pixels );
}
static void APIENTRY logGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params )
{
	SIG( "glGetTexLevelParameterfv" );
	dllGetTexLevelParameterfv( target, level, pname, params );
}

static void APIENTRY logGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params)
{
	SIG( "glGetTexLevelParameteriv" );
	dllGetTexLevelParameteriv( target, level, pname, params );
}

static void APIENTRY logGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params)
{
	SIG( "glGetTexParameterfv" );
	dllGetTexParameterfv( target, pname, params );
}

static void APIENTRY logGetTexParameteriv(GLenum target, GLenum pname, GLint *params)
{
	SIG( "glGetTexParameteriv" );
	dllGetTexParameteriv( target, pname, params );
}

static void APIENTRY logHint(GLenum target, GLenum mode)
{
	fprintf( glw_state.log_fp, "glHint( 0x%x, 0x%x )\n", target, mode );
	dllHint( target, mode );
}

static void APIENTRY logIndexMask(GLuint mask)
{
	SIG( "glIndexMask" );
	dllIndexMask( mask );
}

static void APIENTRY logIndexPointer(GLenum type, GLsizei stride, const void *pointer)
{
	SIG( "glIndexPointer" );
	dllIndexPointer( type, stride, pointer );
}

static void APIENTRY logIndexd(GLdouble c)
{
	SIG( "glIndexd" );
	dllIndexd( c );
}

static void APIENTRY logIndexdv(const GLdouble *c)
{
	SIG( "glIndexdv" );
	dllIndexdv( c );
}

static void APIENTRY logIndexf(GLfloat c)
{
	SIG( "glIndexf" );
	dllIndexf( c );
}

static void APIENTRY logIndexfv(const GLfloat *c)
{
	SIG( "glIndexfv" );
	dllIndexfv( c );
}

static void APIENTRY logIndexi(GLint c)
{
	SIG( "glIndexi" );
	dllIndexi( c );
}

static void APIENTRY logIndexiv(const GLint *c)
{
	SIG( "glIndexiv" );
	dllIndexiv( c );
}

static void APIENTRY logIndexs(GLshort c)
{
	SIG( "glIndexs" );
	dllIndexs( c );
}

static void APIENTRY logIndexsv(const GLshort *c)
{
	SIG( "glIndexsv" );
	dllIndexsv( c );
}

static void APIENTRY logIndexub(GLubyte c)
{
	SIG( "glIndexub" );
	dllIndexub( c );
}

static void APIENTRY logIndexubv(const GLubyte *c)
{
	SIG( "glIndexubv" );
	dllIndexubv( c );
}

static void APIENTRY logInitNames(void)
{
	SIG( "glInitNames" );
	dllInitNames();
}

static void APIENTRY logInterleavedArrays(GLenum format, GLsizei stride, const void *pointer)
{
	SIG( "glInterleavedArrays" );
	dllInterleavedArrays( format, stride, pointer );
}

static GLboolean APIENTRY logIsEnabled(GLenum cap)
{
	SIG( "glIsEnabled" );
	return dllIsEnabled( cap );
}
static GLboolean APIENTRY logIsList(GLuint list)
{
	SIG( "glIsList" );
	return dllIsList( list );
}
static GLboolean APIENTRY logIsTexture(GLuint texture)
{
	SIG( "glIsTexture" );
	return dllIsTexture( texture );
}

static void APIENTRY logLightModelf(GLenum pname, GLfloat param)
{
	SIG( "glLightModelf" );
	dllLightModelf( pname, param );
}

static void APIENTRY logLightModelfv(GLenum pname, const GLfloat *params)
{
	SIG( "glLightModelfv" );
	dllLightModelfv( pname, params );
}

static void APIENTRY logLightModeli(GLenum pname, GLint param)
{
	SIG( "glLightModeli" );
	dllLightModeli( pname, param );

}

static void APIENTRY logLightModeliv(GLenum pname, const GLint *params)
{
	SIG( "glLightModeliv" );
	dllLightModeliv( pname, params );
}

static void APIENTRY logLightf(GLenum light, GLenum pname, GLfloat param)
{
	SIG( "glLightf" );
	dllLightf( light, pname, param );
}

static void APIENTRY logLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	SIG( "glLightfv" );
	dllLightfv( light, pname, params );
}

static void APIENTRY logLighti(GLenum light, GLenum pname, GLint param)
{
	SIG( "glLighti" );
	dllLighti( light, pname, param );
}

static void APIENTRY logLightiv(GLenum light, GLenum pname, const GLint *params)
{
	SIG( "glLightiv" );
	dllLightiv( light, pname, params );
}

static void APIENTRY logLineStipple(GLint factor, GLushort pattern)
{
	SIG( "glLineStipple" );
	dllLineStipple( factor, pattern );
}

static void APIENTRY logLineWidth(GLfloat width)
{
	SIG( "glLineWidth" );
	dllLineWidth( width );
}

static void APIENTRY logListBase(GLuint base)
{
	SIG( "glListBase" );
	dllListBase( base );
}

static void APIENTRY logLoadIdentity(void)
{
	SIG( "glLoadIdentity" );
	dllLoadIdentity();
}

static void APIENTRY logLoadMatrixd(const GLdouble *m)
{
	SIG( "glLoadMatrixd" );
	dllLoadMatrixd( m );
}

static void APIENTRY logLoadMatrixf(const GLfloat *m)
{
	SIG( "glLoadMatrixf" );
	dllLoadMatrixf( m );
}

static void APIENTRY logLoadName(GLuint name)
{
	SIG( "glLoadName" );
	dllLoadName( name );
}

static void APIENTRY logLogicOp(GLenum opcode)
{
	SIG( "glLogicOp" );
	dllLogicOp( opcode );
}

static void APIENTRY logMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points)
{
	SIG( "glMap1d" );
	dllMap1d( target, u1, u2, stride, order, points );
}

static void APIENTRY logMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points)
{
	SIG( "glMap1f" );
	dllMap1f( target, u1, u2, stride, order, points );
}

static void APIENTRY logMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points)
{
	SIG( "glMap2d" );
	dllMap2d( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points );
}

static void APIENTRY logMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points)
{
	SIG( "glMap2f" );
	dllMap2f( target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points );
}

static void APIENTRY logMapGrid1d(GLint un, GLdouble u1, GLdouble u2)
{
	SIG( "glMapGrid1d" );
	dllMapGrid1d( un, u1, u2 );
}

static void APIENTRY logMapGrid1f(GLint un, GLfloat u1, GLfloat u2)
{
	SIG( "glMapGrid1f" );
	dllMapGrid1f( un, u1, u2 );
}

static void APIENTRY logMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)
{
	SIG( "glMapGrid2d" );
	dllMapGrid2d( un, u1, u2, vn, v1, v2 );
}
static void APIENTRY logMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)
{
	SIG( "glMapGrid2f" );
	dllMapGrid2f( un, u1, u2, vn, v1, v2 );
}
static void APIENTRY logMaterialf(GLenum face, GLenum pname, GLfloat param)
{
	SIG( "glMaterialf" );
	dllMaterialf( face, pname, param );
}
static void APIENTRY logMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	SIG( "glMaterialfv" );
	dllMaterialfv( face, pname, params );
}

static void APIENTRY logMateriali(GLenum face, GLenum pname, GLint param)
{
	SIG( "glMateriali" );
	dllMateriali( face, pname, param );
}

static void APIENTRY logMaterialiv(GLenum face, GLenum pname, const GLint *params)
{
	SIG( "glMaterialiv" );
	dllMaterialiv( face, pname, params );
}

static void APIENTRY logMatrixMode(GLenum mode)
{
	SIG( "glMatrixMode" );
	dllMatrixMode( mode );
}

static void APIENTRY logMultMatrixd(const GLdouble *m)
{
	SIG( "glMultMatrixd" );
	dllMultMatrixd( m );
}

static void APIENTRY logMultMatrixf(const GLfloat *m)
{
	SIG( "glMultMatrixf" );
	dllMultMatrixf( m );
}

static void APIENTRY logNewList(GLuint list, GLenum mode)
{
	SIG( "glNewList" );
	dllNewList( list, mode );
}

static void APIENTRY logNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)
{
	SIG ("glNormal3b" );
	dllNormal3b( nx, ny, nz );
}

static void APIENTRY logNormal3bv(const GLbyte *v)
{
	SIG( "glNormal3bv" );
	dllNormal3bv( v );
}

static void APIENTRY logNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)
{
	SIG( "glNormal3d" );
	dllNormal3d( nx, ny, nz );
}

static void APIENTRY logNormal3dv(const GLdouble *v)
{
	SIG( "glNormal3dv" );
	dllNormal3dv( v );
}

static void APIENTRY logNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)
{
	SIG( "glNormal3f" );
	dllNormal3f( nx, ny, nz );
}

static void APIENTRY logNormal3fv(const GLfloat *v)
{
	SIG( "glNormal3fv" );
	dllNormal3fv( v );
}
static void APIENTRY logNormal3i(GLint nx, GLint ny, GLint nz)
{
	SIG( "glNormal3i" );
	dllNormal3i( nx, ny, nz );
}
static void APIENTRY logNormal3iv(const GLint *v)
{
	SIG( "glNormal3iv" );
	dllNormal3iv( v );
}
static void APIENTRY logNormal3s(GLshort nx, GLshort ny, GLshort nz)
{
	SIG( "glNormal3s" );
	dllNormal3s( nx, ny, nz );
}
static void APIENTRY logNormal3sv(const GLshort *v)
{
	SIG( "glNormal3sv" );
	dllNormal3sv( v );
}
static void APIENTRY logNormalPointer(GLenum type, GLsizei stride, const void *pointer)
{
	SIG( "glNormalPointer" );
	dllNormalPointer( type, stride, pointer );
}
static void APIENTRY logOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	SIG( "glOrtho" );
	dllOrtho( left, right, bottom, top, zNear, zFar );
}

static void APIENTRY logPassThrough(GLfloat token)
{
	SIG( "glPassThrough" );
	dllPassThrough( token );
}

static void APIENTRY logPixelMapfv(GLenum map, GLsizei mapsize, const GLfloat *values)
{
	SIG( "glPixelMapfv" );
	dllPixelMapfv( map, mapsize, values );
}

static void APIENTRY logPixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values)
{
	SIG( "glPixelMapuiv" );
	dllPixelMapuiv( map, mapsize, values );
}

static void APIENTRY logPixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values)
{
	SIG( "glPixelMapusv" );
	dllPixelMapusv( map, mapsize, values );
}
static void APIENTRY logPixelStoref(GLenum pname, GLfloat param)
{
	SIG( "glPixelStoref" );
	dllPixelStoref( pname, param );
}
static void APIENTRY logPixelStorei(GLenum pname, GLint param)
{
	SIG( "glPixelStorei" );
	dllPixelStorei( pname, param );
}
static void APIENTRY logPixelTransferf(GLenum pname, GLfloat param)
{
	SIG( "glPixelTransferf" );
	dllPixelTransferf( pname, param );
}

static void APIENTRY logPixelTransferi(GLenum pname, GLint param)
{
	SIG( "glPixelTransferi" );
	dllPixelTransferi( pname, param );
}

static void APIENTRY logPixelZoom(GLfloat xfactor, GLfloat yfactor)
{
	SIG( "glPixelZoom" );
	dllPixelZoom( xfactor, yfactor );
}

static void APIENTRY logPointSize(GLfloat size)
{
	SIG( "glPointSize" );
	dllPointSize( size );
}

static void APIENTRY logPolygonMode(GLenum face, GLenum mode)
{
	fprintf( glw_state.log_fp, "glPolygonMode( 0x%x, 0x%x )\n", face, mode );
	dllPolygonMode( face, mode );
}

static void APIENTRY logPolygonOffset(GLfloat factor, GLfloat units)
{
	SIG( "glPolygonOffset" );
	dllPolygonOffset( factor, units );
}
static void APIENTRY logPolygonStipple(const GLubyte *mask )
{
	SIG( "glPolygonStipple" );
	dllPolygonStipple( mask );
}
static void APIENTRY logPopAttrib(void)
{
	SIG( "glPopAttrib" );
	dllPopAttrib();
}

static void APIENTRY logPopClientAttrib(void)
{
	SIG( "glPopClientAttrib" );
	dllPopClientAttrib();
}

static void APIENTRY logPopMatrix(void)
{
	SIG( "glPopMatrix" );
	dllPopMatrix();
}

static void APIENTRY logPopName(void)
{
	SIG( "glPopName" );
	dllPopName();
}

static void APIENTRY logPrioritizeTextures(GLsizei n, const GLuint *textures, const GLclampf *priorities)
{
	SIG( "glPrioritizeTextures" );
	dllPrioritizeTextures( n, textures, priorities );
}

static void APIENTRY logPushAttrib(GLbitfield mask)
{
	SIG( "glPushAttrib" );
	dllPushAttrib( mask );
}

static void APIENTRY logPushClientAttrib(GLbitfield mask)
{
	SIG( "glPushClientAttrib" );
	dllPushClientAttrib( mask );
}

static void APIENTRY logPushMatrix(void)
{
	SIG( "glPushMatrix" );
	dllPushMatrix();
}

static void APIENTRY logPushName(GLuint name)
{
	SIG( "glPushName" );
	dllPushName( name );
}

static void APIENTRY logRasterPos2d(GLdouble x, GLdouble y)
{
	SIG ("glRasterPot2d" );
	dllRasterPos2d( x, y );
}

static void APIENTRY logRasterPos2dv(const GLdouble *v)
{
	SIG( "glRasterPos2dv" );
	dllRasterPos2dv( v );
}

static void APIENTRY logRasterPos2f(GLfloat x, GLfloat y)
{
	SIG( "glRasterPos2f" );
	dllRasterPos2f( x, y );
}
static void APIENTRY logRasterPos2fv(const GLfloat *v)
{
	SIG( "glRasterPos2dv" );
	dllRasterPos2fv( v );
}
static void APIENTRY logRasterPos2i(GLint x, GLint y)
{
	SIG( "glRasterPos2if" );
	dllRasterPos2i( x, y );
}
static void APIENTRY logRasterPos2iv(const GLint *v)
{
	SIG( "glRasterPos2iv" );
	dllRasterPos2iv( v );
}
static void APIENTRY logRasterPos2s(GLshort x, GLshort y)
{
	SIG( "glRasterPos2s" );
	dllRasterPos2s( x, y );
}
static void APIENTRY logRasterPos2sv(const GLshort *v)
{
	SIG( "glRasterPos2sv" );
	dllRasterPos2sv( v );
}
static void APIENTRY logRasterPos3d(GLdouble x, GLdouble y, GLdouble z)
{
	SIG( "glRasterPos3d" );
	dllRasterPos3d( x, y, z );
}
static void APIENTRY logRasterPos3dv(const GLdouble *v)
{
	SIG( "glRasterPos3dv" );
	dllRasterPos3dv( v );
}
static void APIENTRY logRasterPos3f(GLfloat x, GLfloat y, GLfloat z)
{
	SIG( "glRasterPos3f" );
	dllRasterPos3f( x, y, z );
}
static void APIENTRY logRasterPos3fv(const GLfloat *v)
{
	SIG( "glRasterPos3fv" );
	dllRasterPos3fv( v );
}
static void APIENTRY logRasterPos3i(GLint x, GLint y, GLint z)
{
	SIG( "glRasterPos3i" );
	dllRasterPos3i( x, y, z );
}
static void APIENTRY logRasterPos3iv(const GLint *v)
{
	SIG( "glRasterPos3iv" );
	dllRasterPos3iv( v );
}
static void APIENTRY logRasterPos3s(GLshort x, GLshort y, GLshort z)
{
	SIG( "glRasterPos3s" );
	dllRasterPos3s( x, y, z );
}
static void APIENTRY logRasterPos3sv(const GLshort *v)
{
	SIG( "glRasterPos3sv" );
	dllRasterPos3sv( v );
}
static void APIENTRY logRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
	SIG( "glRasterPos4d" );
	dllRasterPos4d( x, y, z, w );
}
static void APIENTRY logRasterPos4dv(const GLdouble *v)
{
	SIG( "glRasterPos4dv" );
	dllRasterPos4dv( v );
}
static void APIENTRY logRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	SIG( "glRasterPos4f" );
	dllRasterPos4f( x, y, z, w );
}
static void APIENTRY logRasterPos4fv(const GLfloat *v)
{
	SIG( "glRasterPos4fv" );
	dllRasterPos4fv( v );
}
static void APIENTRY logRasterPos4i(GLint x, GLint y, GLint z, GLint w)
{
	SIG( "glRasterPos4i" );
	dllRasterPos4i( x, y, z, w );
}
static void APIENTRY logRasterPos4iv(const GLint *v)
{
	SIG( "glRasterPos4iv" );
	dllRasterPos4iv( v );
}
static void APIENTRY logRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
	SIG( "glRasterPos4s" );
	dllRasterPos4s( x, y, z, w );
}
static void APIENTRY logRasterPos4sv(const GLshort *v)
{
	SIG( "glRasterPos4sv" );
	dllRasterPos4sv( v );
}
static void APIENTRY logReadBuffer(GLenum mode)
{
	SIG( "glReadBuffer" );
	dllReadBuffer( mode );
}
static void APIENTRY logReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels)
{
	SIG( "glReadPixels" );
	dllReadPixels( x, y, width, height, format, type, pixels );
}

static void APIENTRY logRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
{
	SIG( "glRectd" );
	dllRectd( x1, y1, x2, y2 );
}

static void APIENTRY logRectdv(const GLdouble *v1, const GLdouble *v2)
{
	SIG( "glRectdv" );
	dllRectdv( v1, v2 );
}

static void APIENTRY logRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	SIG( "glRectf" );
	dllRectf( x1, y1, x2, y2 );
}

static void APIENTRY logRectfv(const GLfloat *v1, const GLfloat *v2)
{
	SIG( "glRectfv" );
	dllRectfv( v1, v2 );
}
static void APIENTRY logRecti(GLint x1, GLint y1, GLint x2, GLint y2)
{
	SIG( "glRecti" );
	dllRecti( x1, y1, x2, y2 );
}
static void APIENTRY logRectiv(const GLint *v1, const GLint *v2)
{
	SIG( "glRectiv" );
	dllRectiv( v1, v2 );
}
static void APIENTRY logRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)
{
	SIG( "glRects" );
	dllRects( x1, y1, x2, y2 );
}
static void APIENTRY logRectsv(const GLshort *v1, const GLshort *v2)
{
	SIG( "glRectsv" );
	dllRectsv( v1, v2 );
}
static GLint APIENTRY logRenderMode(GLenum mode)
{
	SIG( "glRenderMode" );
	return dllRenderMode( mode );
}
static void APIENTRY logRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
	SIG( "glRotated" );
	dllRotated( angle, x, y, z );
}

static void APIENTRY logRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	SIG( "glRotatef" );
	dllRotatef( angle, x, y, z );
}

static void APIENTRY logScaled(GLdouble x, GLdouble y, GLdouble z)
{
	SIG( "glScaled" );
	dllScaled( x, y, z );
}

static void APIENTRY logScalef(GLfloat x, GLfloat y, GLfloat z)
{
	SIG( "glScalef" );
	dllScalef( x, y, z );
}

static void APIENTRY logScissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	SIG( "glScissor" );
	dllScissor( x, y, width, height );
}

static void APIENTRY logSelectBuffer(GLsizei size, GLuint *buffer)
{
	SIG( "glSelectBuffer" );
	dllSelectBuffer( size, buffer );
}

static void APIENTRY logShadeModel(GLenum mode)
{
	SIG( "glShadeModel" );
	dllShadeModel( mode );
}

static void APIENTRY logStencilFunc(GLenum func, GLint ref, GLuint mask)
{
	SIG( "glStencilFunc" );
	dllStencilFunc( func, ref, mask );
}

static void APIENTRY logStencilMask(GLuint mask)
{
	SIG( "glStencilMask" );
	dllStencilMask( mask );
}

static void APIENTRY logStencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	SIG( "glStencilOp" );
	dllStencilOp( fail, zfail, zpass );
}

static void APIENTRY logTexCoord1d(GLdouble s)
{
	SIG( "glTexCoord1d" );
	dllTexCoord1d( s );
}

static void APIENTRY logTexCoord1dv(const GLdouble *v)
{
	SIG( "glTexCoord1dv" );
	dllTexCoord1dv( v );
}

static void APIENTRY logTexCoord1f(GLfloat s)
{
	SIG( "glTexCoord1f" );
	dllTexCoord1f( s );
}
static void APIENTRY logTexCoord1fv(const GLfloat *v)
{
	SIG( "glTexCoord1fv" );
	dllTexCoord1fv( v );
}
static void APIENTRY logTexCoord1i(GLint s)
{
	SIG( "glTexCoord1i" );
	dllTexCoord1i( s );
}
static void APIENTRY logTexCoord1iv(const GLint *v)
{
	SIG( "glTexCoord1iv" );
	dllTexCoord1iv( v );
}
static void APIENTRY logTexCoord1s(GLshort s)
{
	SIG( "glTexCoord1s" );
	dllTexCoord1s( s );
}
static void APIENTRY logTexCoord1sv(const GLshort *v)
{
	SIG( "glTexCoord1sv" );
	dllTexCoord1sv( v );
}
static void APIENTRY logTexCoord2d(GLdouble s, GLdouble t)
{
	SIG( "glTexCoord2d" );
	dllTexCoord2d( s, t );
}

static void APIENTRY logTexCoord2dv(const GLdouble *v)
{
	SIG( "glTexCoord2dv" );
	dllTexCoord2dv( v );
}
static void APIENTRY logTexCoord2f(GLfloat s, GLfloat t)
{
	SIG( "glTexCoord2f" );
	dllTexCoord2f( s, t );
}
static void APIENTRY logTexCoord2fv(const GLfloat *v)
{
	SIG( "glTexCoord2fv" );
	dllTexCoord2fv( v );
}
static void APIENTRY logTexCoord2i(GLint s, GLint t)
{
	SIG( "glTexCoord2i" );
	dllTexCoord2i( s, t );
}
static void APIENTRY logTexCoord2iv(const GLint *v)
{
	SIG( "glTexCoord2iv" );
	dllTexCoord2iv( v );
}
static void APIENTRY logTexCoord2s(GLshort s, GLshort t)
{
	SIG( "glTexCoord2s" );
	dllTexCoord2s( s, t );
}
static void APIENTRY logTexCoord2sv(const GLshort *v)
{
	SIG( "glTexCoord2sv" );
	dllTexCoord2sv( v );
}
static void APIENTRY logTexCoord3d(GLdouble s, GLdouble t, GLdouble r)
{
	SIG( "glTexCoord3d" );
	dllTexCoord3d( s, t, r );
}
static void APIENTRY logTexCoord3dv(const GLdouble *v)
{
	SIG( "glTexCoord3dv" );
	dllTexCoord3dv( v );
}
static void APIENTRY logTexCoord3f(GLfloat s, GLfloat t, GLfloat r)
{
	SIG( "glTexCoord3f" );
	dllTexCoord3f( s, t, r );
}
static void APIENTRY logTexCoord3fv(const GLfloat *v)
{
	SIG( "glTexCoord3fv" );
	dllTexCoord3fv( v );
}
static void APIENTRY logTexCoord3i(GLint s, GLint t, GLint r)
{
	SIG( "glTexCoord3i" );
	dllTexCoord3i( s, t, r );
}
static void APIENTRY logTexCoord3iv(const GLint *v)
{
	SIG( "glTexCoord3iv" );
	dllTexCoord3iv( v );
}
static void APIENTRY logTexCoord3s(GLshort s, GLshort t, GLshort r)
{
	SIG( "glTexCoord3s" );
	dllTexCoord3s( s, t, r );
}
static void APIENTRY logTexCoord3sv(const GLshort *v)
{
	SIG( "glTexCoord3sv" );
	dllTexCoord3sv( v );
}
static void APIENTRY logTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)
{
	SIG( "glTexCoord4d" );
	dllTexCoord4d( s, t, r, q );
}
static void APIENTRY logTexCoord4dv(const GLdouble *v)
{
	SIG( "glTexCoord4dv" );
	dllTexCoord4dv( v );
}
static void APIENTRY logTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)
{
	SIG( "glTexCoord4f" );
	dllTexCoord4f( s, t, r, q );
}
static void APIENTRY logTexCoord4fv(const GLfloat *v)
{
	SIG( "glTexCoord4fv" );
	dllTexCoord4fv( v );
}
static void APIENTRY logTexCoord4i(GLint s, GLint t, GLint r, GLint q)
{
	SIG( "glTexCoord4i" );
	dllTexCoord4i( s, t, r, q );
}
static void APIENTRY logTexCoord4iv(const GLint *v)
{
	SIG( "glTexCoord4iv" );
	dllTexCoord4iv( v );
}
static void APIENTRY logTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)
{
	SIG( "glTexCoord4s" );
	dllTexCoord4s( s, t, r, q );
}
static void APIENTRY logTexCoord4sv(const GLshort *v)
{
	SIG( "glTexCoord4sv" );
	dllTexCoord4sv( v );
}
static void APIENTRY logTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	SIG( "glTexCoordPointer" );
	dllTexCoordPointer( size, type, stride, pointer );
}

static void APIENTRY logTexEnvf(GLenum target, GLenum pname, GLfloat param)
{
	fprintf( glw_state.log_fp, "glTexEnvf( 0x%x, 0x%x, %f )\n", target, pname, param );
	dllTexEnvf( target, pname, param );
}

static void APIENTRY logTexEnvfv(GLenum target, GLenum pname, const GLfloat *params)
{
	SIG( "glTexEnvfv" );
	dllTexEnvfv( target, pname, params );
}

static void APIENTRY logTexEnvi(GLenum target, GLenum pname, GLint param)
{
	fprintf( glw_state.log_fp, "glTexEnvi( 0x%x, 0x%x, 0x%x )\n", target, pname, param );
	dllTexEnvi( target, pname, param );
}
static void APIENTRY logTexEnviv(GLenum target, GLenum pname, const GLint *params)
{
	SIG( "glTexEnviv" );
	dllTexEnviv( target, pname, params );
}

static void APIENTRY logTexGend(GLenum coord, GLenum pname, GLdouble param)
{
	SIG( "glTexGend" );
	dllTexGend( coord, pname, param );
}

static void APIENTRY logTexGendv(GLenum coord, GLenum pname, const GLdouble *params)
{
	SIG( "glTexGendv" );
	dllTexGendv( coord, pname, params );
}

static void APIENTRY logTexGenf(GLenum coord, GLenum pname, GLfloat param)
{
	SIG( "glTexGenf" );
	dllTexGenf( coord, pname, param );
}
static void APIENTRY logTexGenfv(GLenum coord, GLenum pname, const GLfloat *params)
{
	SIG( "glTexGenfv" );
	dllTexGenfv( coord, pname, params );
}
static void APIENTRY logTexGeni(GLenum coord, GLenum pname, GLint param)
{
	SIG( "glTexGeni" );
	dllTexGeni( coord, pname, param );
}
static void APIENTRY logTexGeniv(GLenum coord, GLenum pname, const GLint *params)
{
	SIG( "glTexGeniv" );
	dllTexGeniv( coord, pname, params );
}
static void APIENTRY logTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels)
{
	SIG( "glTexImage1D" );
	dllTexImage1D( target, level, internalformat, width, border, format, type, pixels );
}
static void APIENTRY logTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
	SIG( "glTexImage2D" );
	dllTexImage2D( target, level, internalformat, width, height, border, format, type, pixels );
}

static void APIENTRY logTexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	fprintf( glw_state.log_fp, "glTexParameterf( 0x%x, 0x%x, %f )\n", target, pname, param );
	dllTexParameterf( target, pname, param );
}

static void APIENTRY logTexParameterfv(GLenum target, GLenum pname, const GLfloat *params)
{
	SIG( "glTexParameterfv" );
	dllTexParameterfv( target, pname, params );
}
static void APIENTRY logTexParameteri(GLenum target, GLenum pname, GLint param)
{
	fprintf( glw_state.log_fp, "glTexParameteri( 0x%x, 0x%x, 0x%x )\n", target, pname, param );
	dllTexParameteri( target, pname, param );
}
static void APIENTRY logTexParameteriv(GLenum target, GLenum pname, const GLint *params)
{
	SIG( "glTexParameteriv" );
	dllTexParameteriv( target, pname, params );
}
static void APIENTRY logTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)
{
	SIG( "glTexSubImage1D" );
	dllTexSubImage1D( target, level, xoffset, width, format, type, pixels );
}
static void APIENTRY logTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)
{
	SIG( "glTexSubImage2D" );
	dllTexSubImage2D( target, level, xoffset, yoffset, width, height, format, type, pixels );
}
static void APIENTRY logTranslated(GLdouble x, GLdouble y, GLdouble z)
{
	SIG( "glTranslated" );
	dllTranslated( x, y, z );
}

static void APIENTRY logTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	SIG( "glTranslatef" );
	dllTranslatef( x, y, z );
}

static void APIENTRY logVertex2d(GLdouble x, GLdouble y)
{
	SIG( "glVertex2d" );
	dllVertex2d( x, y );
}

static void APIENTRY logVertex2dv(const GLdouble *v)
{
	SIG( "glVertex2dv" );
	dllVertex2dv( v );
}
static void APIENTRY logVertex2f(GLfloat x, GLfloat y)
{
	SIG( "glVertex2f" );
	dllVertex2f( x, y );
}
static void APIENTRY logVertex2fv(const GLfloat *v)
{
	SIG( "glVertex2fv" );
	dllVertex2fv( v );
}
static void APIENTRY logVertex2i(GLint x, GLint y)
{
	SIG( "glVertex2i" );
	dllVertex2i( x, y );
}
static void APIENTRY logVertex2iv(const GLint *v)
{
	SIG( "glVertex2iv" );
	dllVertex2iv( v );
}
static void APIENTRY logVertex2s(GLshort x, GLshort y)
{
	SIG( "glVertex2s" );
	dllVertex2s( x, y );
}
static void APIENTRY logVertex2sv(const GLshort *v)
{
	SIG( "glVertex2sv" );
	dllVertex2sv( v );
}
static void APIENTRY logVertex3d(GLdouble x, GLdouble y, GLdouble z)
{
	SIG( "glVertex3d" );
	dllVertex3d( x, y, z );
}
static void APIENTRY logVertex3dv(const GLdouble *v)
{
	SIG( "glVertex3dv" );
	dllVertex3dv( v );
}
static void APIENTRY logVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	SIG( "glVertex3f" );
	dllVertex3f( x, y, z );
}
static void APIENTRY logVertex3fv(const GLfloat *v)
{
	SIG( "glVertex3fv" );
	dllVertex3fv( v );
}
static void APIENTRY logVertex3i(GLint x, GLint y, GLint z)
{
	SIG( "glVertex3i" );
	dllVertex3i( x, y, z );
}
static void APIENTRY logVertex3iv(const GLint *v)
{
	SIG( "glVertex3iv" );
	dllVertex3iv( v );
}
static void APIENTRY logVertex3s(GLshort x, GLshort y, GLshort z)
{
	SIG( "glVertex3s" );
	dllVertex3s( x, y, z );
}
static void APIENTRY logVertex3sv(const GLshort *v)
{
	SIG( "glVertex3sv" );
	dllVertex3sv( v );
}
static void APIENTRY logVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)
{
	SIG( "glVertex4d" );
	dllVertex4d( x, y, z, w );
}
static void APIENTRY logVertex4dv(const GLdouble *v)
{
	SIG( "glVertex4dv" );
	dllVertex4dv( v );
}
static void APIENTRY logVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	SIG( "glVertex4f" );
	dllVertex4f( x, y, z, w );
}
static void APIENTRY logVertex4fv(const GLfloat *v)
{
	SIG( "glVertex4fv" );
	dllVertex4fv( v );
}
static void APIENTRY logVertex4i(GLint x, GLint y, GLint z, GLint w)
{
	SIG( "glVertex4i" );
	dllVertex4i( x, y, z, w );
}
static void APIENTRY logVertex4iv(const GLint *v)
{
	SIG( "glVertex4iv" );
	dllVertex4iv( v );
}
static void APIENTRY logVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)
{
	SIG( "glVertex4s" );
	dllVertex4s( x, y, z, w );
}
static void APIENTRY logVertex4sv(const GLshort *v)
{
	SIG( "glVertex4sv" );
	dllVertex4sv( v );
}
static void APIENTRY logVertexPointer(GLint size, GLenum type, GLsizei stride, const void *pointer)
{
	SIG( "glVertexPointer" );
	dllVertexPointer( size, type, stride, pointer );
}
static void APIENTRY logViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	SIG( "glViewport" );
	dllViewport( x, y, width, height );
}

/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void QGL_Shutdown( void )
{
	if ( glw_state.hinstOpenGL )
	{
		FreeLibrary( glw_state.hinstOpenGL );
		glw_state.hinstOpenGL = NULL;
	}

	glw_state.hinstOpenGL = NULL;

	glAccum                     = NULL;
	glAlphaFunc                 = NULL;
	glAreTexturesResident       = NULL;
	glArrayElement              = NULL;
	glBegin                     = NULL;
	glBindTexture               = NULL;
	glBitmap                    = NULL;
	glBlendFunc                 = NULL;
	glCallList                  = NULL;
	glCallLists                 = NULL;
	glClear                     = NULL;
	glClearAccum                = NULL;
	glClearColor                = NULL;
	glClearDepth                = NULL;
	glClearIndex                = NULL;
	glClearStencil              = NULL;
	glClipPlane                 = NULL;
	glColor3b                   = NULL;
	glColor3bv                  = NULL;
	glColor3d                   = NULL;
	glColor3dv                  = NULL;
	glColor3f                   = NULL;
	glColor3fv                  = NULL;
	glColor3i                   = NULL;
	glColor3iv                  = NULL;
	glColor3s                   = NULL;
	glColor3sv                  = NULL;
	glColor3ub                  = NULL;
	glColor3ubv                 = NULL;
	glColor3ui                  = NULL;
	glColor3uiv                 = NULL;
	glColor3us                  = NULL;
	glColor3usv                 = NULL;
	glColor4b                   = NULL;
	glColor4bv                  = NULL;
	glColor4d                   = NULL;
	glColor4dv                  = NULL;
	glColor4f                   = NULL;
	glColor4fv                  = NULL;
	glColor4i                   = NULL;
	glColor4iv                  = NULL;
	glColor4s                   = NULL;
	glColor4sv                  = NULL;
	glColor4ub                  = NULL;
	glColor4ubv                 = NULL;
	glColor4ui                  = NULL;
	glColor4uiv                 = NULL;
	glColor4us                  = NULL;
	glColor4usv                 = NULL;
	glColorMask                 = NULL;
	glColorMaterial             = NULL;
	glColorPointer              = NULL;
	glCopyPixels                = NULL;
	glCopyTexImage1D            = NULL;
	glCopyTexImage2D            = NULL;
	glCopyTexSubImage1D         = NULL;
	glCopyTexSubImage2D         = NULL;
	glCullFace                  = NULL;
	glDeleteLists               = NULL;
	glDeleteTextures            = NULL;
	glDepthFunc                 = NULL;
	glDepthMask                 = NULL;
	glDepthRange                = NULL;
	glDisable                   = NULL;
	glDisableClientState        = NULL;
	glDrawArrays                = NULL;
	glDrawBuffer                = NULL;
	glDrawElements              = NULL;
	glDrawPixels                = NULL;
	glEdgeFlag                  = NULL;
	glEdgeFlagPointer           = NULL;
	glEdgeFlagv                 = NULL;
	glEnable                    = NULL;
	glEnableClientState         = NULL;
	glEnd                       = NULL;
	glEndList                   = NULL;
	glEvalCoord1d               = NULL;
	glEvalCoord1dv              = NULL;
	glEvalCoord1f               = NULL;
	glEvalCoord1fv              = NULL;
	glEvalCoord2d               = NULL;
	glEvalCoord2dv              = NULL;
	glEvalCoord2f               = NULL;
	glEvalCoord2fv              = NULL;
	glEvalMesh1                 = NULL;
	glEvalMesh2                 = NULL;
	glEvalPoint1                = NULL;
	glEvalPoint2                = NULL;
	glFeedbackBuffer            = NULL;
	glFinish                    = NULL;
	glFlush                     = NULL;
	glFogf                      = NULL;
	glFogfv                     = NULL;
	glFogi                      = NULL;
	glFogiv                     = NULL;
	glFrontFace                 = NULL;
	glFrustum                   = NULL;
	glGenLists                  = NULL;
	glGenTextures               = NULL;
	glGetBooleanv               = NULL;
	glGetClipPlane              = NULL;
	glGetDoublev                = NULL;
	glGetError                  = NULL;
	glGetFloatv                 = NULL;
	glGetIntegerv               = NULL;
	glGetLightfv                = NULL;
	glGetLightiv                = NULL;
	glGetMapdv                  = NULL;
	glGetMapfv                  = NULL;
	glGetMapiv                  = NULL;
	glGetMaterialfv             = NULL;
	glGetMaterialiv             = NULL;
	glGetPixelMapfv             = NULL;
	glGetPixelMapuiv            = NULL;
	glGetPixelMapusv            = NULL;
	glGetPointerv               = NULL;
	glGetPolygonStipple         = NULL;
	glGetString                 = NULL;
	glGetTexEnvfv               = NULL;
	glGetTexEnviv               = NULL;
	glGetTexGendv               = NULL;
	glGetTexGenfv               = NULL;
	glGetTexGeniv               = NULL;
	glGetTexImage               = NULL;
	glGetTexLevelParameterfv    = NULL;
	glGetTexLevelParameteriv    = NULL;
	glGetTexParameterfv         = NULL;
	glGetTexParameteriv         = NULL;
	glHint                      = NULL;
	glIndexMask                 = NULL;
	glIndexPointer              = NULL;
	glIndexd                    = NULL;
	glIndexdv                   = NULL;
	glIndexf                    = NULL;
	glIndexfv                   = NULL;
	glIndexi                    = NULL;
	glIndexiv                   = NULL;
	glIndexs                    = NULL;
	glIndexsv                   = NULL;
	glIndexub                   = NULL;
	glIndexubv                  = NULL;
	glInitNames                 = NULL;
	glInterleavedArrays         = NULL;
	glIsEnabled                 = NULL;
	glIsList                    = NULL;
	glIsTexture                 = NULL;
	glLightModelf               = NULL;
	glLightModelfv              = NULL;
	glLightModeli               = NULL;
	glLightModeliv              = NULL;
	glLightf                    = NULL;
	glLightfv                   = NULL;
	glLighti                    = NULL;
	glLightiv                   = NULL;
	glLineStipple               = NULL;
	glLineWidth                 = NULL;
	glListBase                  = NULL;
	glLoadIdentity              = NULL;
	glLoadMatrixd               = NULL;
	glLoadMatrixf               = NULL;
	glLoadName                  = NULL;
	glLogicOp                   = NULL;
	glMap1d                     = NULL;
	glMap1f                     = NULL;
	glMap2d                     = NULL;
	glMap2f                     = NULL;
	glMapGrid1d                 = NULL;
	glMapGrid1f                 = NULL;
	glMapGrid2d                 = NULL;
	glMapGrid2f                 = NULL;
	glMaterialf                 = NULL;
	glMaterialfv                = NULL;
	glMateriali                 = NULL;
	glMaterialiv                = NULL;
	glMatrixMode                = NULL;
	glMultMatrixd               = NULL;
	glMultMatrixf               = NULL;
	glNewList                   = NULL;
	glNormal3b                  = NULL;
	glNormal3bv                 = NULL;
	glNormal3d                  = NULL;
	glNormal3dv                 = NULL;
	glNormal3f                  = NULL;
	glNormal3fv                 = NULL;
	glNormal3i                  = NULL;
	glNormal3iv                 = NULL;
	glNormal3s                  = NULL;
	glNormal3sv                 = NULL;
	glNormalPointer             = NULL;
	glOrtho                     = NULL;
	glPassThrough               = NULL;
	glPixelMapfv                = NULL;
	glPixelMapuiv               = NULL;
	glPixelMapusv               = NULL;
	glPixelStoref               = NULL;
	glPixelStorei               = NULL;
	glPixelTransferf            = NULL;
	glPixelTransferi            = NULL;
	glPixelZoom                 = NULL;
	glPointSize                 = NULL;
	glPolygonMode               = NULL;
	glPolygonOffset             = NULL;
	glPolygonStipple            = NULL;
	glPopAttrib                 = NULL;
	glPopClientAttrib           = NULL;
	glPopMatrix                 = NULL;
	glPopName                   = NULL;
	glPrioritizeTextures        = NULL;
	glPushAttrib                = NULL;
	glPushClientAttrib          = NULL;
	glPushMatrix                = NULL;
	glPushName                  = NULL;
	glRasterPos2d               = NULL;
	glRasterPos2dv              = NULL;
	glRasterPos2f               = NULL;
	glRasterPos2fv              = NULL;
	glRasterPos2i               = NULL;
	glRasterPos2iv              = NULL;
	glRasterPos2s               = NULL;
	glRasterPos2sv              = NULL;
	glRasterPos3d               = NULL;
	glRasterPos3dv              = NULL;
	glRasterPos3f               = NULL;
	glRasterPos3fv              = NULL;
	glRasterPos3i               = NULL;
	glRasterPos3iv              = NULL;
	glRasterPos3s               = NULL;
	glRasterPos3sv              = NULL;
	glRasterPos4d               = NULL;
	glRasterPos4dv              = NULL;
	glRasterPos4f               = NULL;
	glRasterPos4fv              = NULL;
	glRasterPos4i               = NULL;
	glRasterPos4iv              = NULL;
	glRasterPos4s               = NULL;
	glRasterPos4sv              = NULL;
	glReadBuffer                = NULL;
	glReadPixels                = NULL;
	glRectd                     = NULL;
	glRectdv                    = NULL;
	glRectf                     = NULL;
	glRectfv                    = NULL;
	glRecti                     = NULL;
	glRectiv                    = NULL;
	glRects                     = NULL;
	glRectsv                    = NULL;
	glRenderMode                = NULL;
	glRotated                   = NULL;
	glRotatef                   = NULL;
	glScaled                    = NULL;
	glScalef                    = NULL;
	glScissor                   = NULL;
	glSelectBuffer              = NULL;
	glShadeModel                = NULL;
	glStencilFunc               = NULL;
	glStencilMask               = NULL;
	glStencilOp                 = NULL;
	glTexCoord1d                = NULL;
	glTexCoord1dv               = NULL;
	glTexCoord1f                = NULL;
	glTexCoord1fv               = NULL;
	glTexCoord1i                = NULL;
	glTexCoord1iv               = NULL;
	glTexCoord1s                = NULL;
	glTexCoord1sv               = NULL;
	glTexCoord2d                = NULL;
	glTexCoord2dv               = NULL;
	glTexCoord2f                = NULL;
	glTexCoord2fv               = NULL;
	glTexCoord2i                = NULL;
	glTexCoord2iv               = NULL;
	glTexCoord2s                = NULL;
	glTexCoord2sv               = NULL;
	glTexCoord3d                = NULL;
	glTexCoord3dv               = NULL;
	glTexCoord3f                = NULL;
	glTexCoord3fv               = NULL;
	glTexCoord3i                = NULL;
	glTexCoord3iv               = NULL;
	glTexCoord3s                = NULL;
	glTexCoord3sv               = NULL;
	glTexCoord4d                = NULL;
	glTexCoord4dv               = NULL;
	glTexCoord4f                = NULL;
	glTexCoord4fv               = NULL;
	glTexCoord4i                = NULL;
	glTexCoord4iv               = NULL;
	glTexCoord4s                = NULL;
	glTexCoord4sv               = NULL;
	glTexCoordPointer           = NULL;
	glTexEnvf                   = NULL;
	glTexEnvfv                  = NULL;
	glTexEnvi                   = NULL;
	glTexEnviv                  = NULL;
	glTexGend                   = NULL;
	glTexGendv                  = NULL;
	glTexGenf                   = NULL;
	glTexGenfv                  = NULL;
	glTexGeni                   = NULL;
	glTexGeniv                  = NULL;
	glTexImage1D                = NULL;
	glTexImage2D                = NULL;
	glTexParameterf             = NULL;
	glTexParameterfv            = NULL;
	glTexParameteri             = NULL;
	glTexParameteriv            = NULL;
	glTexSubImage1D             = NULL;
	glTexSubImage2D             = NULL;
	glTranslated                = NULL;
	glTranslatef                = NULL;
	glVertex2d                  = NULL;
	glVertex2dv                 = NULL;
	glVertex2f                  = NULL;
	glVertex2fv                 = NULL;
	glVertex2i                  = NULL;
	glVertex2iv                 = NULL;
	glVertex2s                  = NULL;
	glVertex2sv                 = NULL;
	glVertex3d                  = NULL;
	glVertex3dv                 = NULL;
	glVertex3f                  = NULL;
	glVertex3fv                 = NULL;
	glVertex3i                  = NULL;
	glVertex3iv                 = NULL;
	glVertex3s                  = NULL;
	glVertex3sv                 = NULL;
	glVertex4d                  = NULL;
	glVertex4dv                 = NULL;
	glVertex4f                  = NULL;
	glVertex4fv                 = NULL;
	glVertex4i                  = NULL;
	glVertex4iv                 = NULL;
	glVertex4s                  = NULL;
	glVertex4sv                 = NULL;
	glVertexPointer             = NULL;
	glViewport                  = NULL;

	qwglCopyContext              = NULL;
	qwglCreateContext            = NULL;
	qwglCreateLayerContext       = NULL;
	qwglDeleteContext            = NULL;
	qwglDescribeLayerPlane       = NULL;
	qwglGetCurrentContext        = NULL;
	qwglGetCurrentDC             = NULL;
	qwglGetLayerPaletteEntries   = NULL;
	qwglGetProcAddress           = NULL;
	qwglMakeCurrent              = NULL;
	qwglRealizeLayerPalette      = NULL;
	qwglSetLayerPaletteEntries   = NULL;
	qwglShareLists               = NULL;
	qwglSwapLayerBuffers         = NULL;
	qwglUseFontBitmaps           = NULL;
	qwglUseFontOutlines          = NULL;

	qwglChoosePixelFormat        = NULL;
	qwglDescribePixelFormat      = NULL;
	qwglGetPixelFormat           = NULL;
	qwglSetPixelFormat           = NULL;
	qwglSwapBuffers              = NULL;

	qwglSwapIntervalEXT	= NULL;

	qwglGetDeviceGammaRampEXT = NULL;
	qwglSetDeviceGammaRampEXT = NULL;
}

#	pragma warning (disable : 4113 4133 4047 )
#	define GPA( a ) GetProcAddress( glw_state.hinstOpenGL, a )

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to 
** the appropriate GL stuff.  In Windows this means doing a 
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
** 
*/
qboolean QGL_Init( const char *dllname )
{
	// update 3Dfx gamma irrespective of underlying DLL
	{
		char envbuffer[1024];
		float g;

		g = 2.00 * ( 0.8 - ( vid_gamma->value - 0.5 ) ) + 1.0F;
		Com_sprintf( envbuffer, sizeof(envbuffer), "SSTV2_GAMMA=%f", g );
		putenv( envbuffer );
		Com_sprintf( envbuffer, sizeof(envbuffer), "SST_GAMMA=%f", g );
		putenv( envbuffer );
	}

	if ( ( glw_state.hinstOpenGL = LoadLibrary( dllname ) ) == 0 )
	{
		char *buf = NULL;

		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &buf, 0, NULL);
		ri.Con_Printf( PRINT_ALL, "%s\n", buf );
		return false;
	}

	gl_config.allow_cds = true;

	glAccum                     = dllAccum = GPA( "glAccum" );
	glAlphaFunc                 = dllAlphaFunc = GPA( "glAlphaFunc" );
	glAreTexturesResident       = dllAreTexturesResident = GPA( "glAreTexturesResident" );
	glArrayElement              = dllArrayElement = GPA( "glArrayElement" );
	glBegin                     = dllBegin = GPA( "glBegin" );
	glBindTexture               = dllBindTexture = GPA( "glBindTexture" );
	glBitmap                    = dllBitmap = GPA( "glBitmap" );
	glBlendFunc                 = dllBlendFunc = GPA( "glBlendFunc" );
	glCallList                  = dllCallList = GPA( "glCallList" );
	glCallLists                 = dllCallLists = GPA( "glCallLists" );
	glClear                     = dllClear = GPA( "glClear" );
	glClearAccum                = dllClearAccum = GPA( "glClearAccum" );
	glClearColor                = dllClearColor = GPA( "glClearColor" );
	glClearDepth                = dllClearDepth = GPA( "glClearDepth" );
	glClearIndex                = dllClearIndex = GPA( "glClearIndex" );
	glClearStencil              = dllClearStencil = GPA( "glClearStencil" );
	glClipPlane                 = dllClipPlane = GPA( "glClipPlane" );
	glColor3b                   = dllColor3b = GPA( "glColor3b" );
	glColor3bv                  = dllColor3bv = GPA( "glColor3bv" );
	glColor3d                   = dllColor3d = GPA( "glColor3d" );
	glColor3dv                  = dllColor3dv = GPA( "glColor3dv" );
	glColor3f                   = dllColor3f = GPA( "glColor3f" );
	glColor3fv                  = dllColor3fv = GPA( "glColor3fv" );
	glColor3i                   = dllColor3i = GPA( "glColor3i" );
	glColor3iv                  = dllColor3iv = GPA( "glColor3iv" );
	glColor3s                   = dllColor3s = GPA( "glColor3s" );
	glColor3sv                  = dllColor3sv = GPA( "glColor3sv" );
	glColor3ub                  = dllColor3ub = GPA( "glColor3ub" );
	glColor3ubv                 = dllColor3ubv = GPA( "glColor3ubv" );
	glColor3ui                  = dllColor3ui = GPA( "glColor3ui" );
	glColor3uiv                 = dllColor3uiv = GPA( "glColor3uiv" );
	glColor3us                  = dllColor3us = GPA( "glColor3us" );
	glColor3usv                 = dllColor3usv = GPA( "glColor3usv" );
	glColor4b                   = dllColor4b = GPA( "glColor4b" );
	glColor4bv                  = dllColor4bv = GPA( "glColor4bv" );
	glColor4d                   = dllColor4d = GPA( "glColor4d" );
	glColor4dv                  = dllColor4dv = GPA( "glColor4dv" );
	glColor4f                   = dllColor4f = GPA( "glColor4f" );
	glColor4fv                  = dllColor4fv = GPA( "glColor4fv" );
	glColor4i                   = dllColor4i = GPA( "glColor4i" );
	glColor4iv                  = dllColor4iv = GPA( "glColor4iv" );
	glColor4s                   = dllColor4s = GPA( "glColor4s" );
	glColor4sv                  = dllColor4sv = GPA( "glColor4sv" );
	glColor4ub                  = dllColor4ub = GPA( "glColor4ub" );
	glColor4ubv                 = dllColor4ubv = GPA( "glColor4ubv" );
	glColor4ui                  = dllColor4ui = GPA( "glColor4ui" );
	glColor4uiv                 = dllColor4uiv = GPA( "glColor4uiv" );
	glColor4us                  = dllColor4us = GPA( "glColor4us" );
	glColor4usv                 = dllColor4usv = GPA( "glColor4usv" );
	glColorMask                 = dllColorMask = GPA( "glColorMask" );
	glColorMaterial             = dllColorMaterial = GPA( "glColorMaterial" );
	glColorPointer              = dllColorPointer = GPA( "glColorPointer" );
	glCopyPixels                = dllCopyPixels = GPA( "glCopyPixels" );
	glCopyTexImage1D            = dllCopyTexImage1D = GPA( "glCopyTexImage1D" );
	glCopyTexImage2D            = dllCopyTexImage2D = GPA( "glCopyTexImage2D" );
	glCopyTexSubImage1D         = dllCopyTexSubImage1D = GPA( "glCopyTexSubImage1D" );
	glCopyTexSubImage2D         = dllCopyTexSubImage2D = GPA( "glCopyTexSubImage2D" );
	glCullFace                  = dllCullFace = GPA( "glCullFace" );
	glDeleteLists               = dllDeleteLists = GPA( "glDeleteLists" );
	glDeleteTextures            = dllDeleteTextures = GPA( "glDeleteTextures" );
	glDepthFunc                 = dllDepthFunc = GPA( "glDepthFunc" );
	glDepthMask                 = dllDepthMask = GPA( "glDepthMask" );
	glDepthRange                = dllDepthRange = GPA( "glDepthRange" );
	glDisable                   = dllDisable = GPA( "glDisable" );
	glDisableClientState        = dllDisableClientState = GPA( "glDisableClientState" );
	glDrawArrays                = dllDrawArrays = GPA( "glDrawArrays" );
	glDrawBuffer                = dllDrawBuffer = GPA( "glDrawBuffer" );
	glDrawElements              = dllDrawElements = GPA( "glDrawElements" );
	glDrawPixels                = dllDrawPixels = GPA( "glDrawPixels" );
	glEdgeFlag                  = dllEdgeFlag = GPA( "glEdgeFlag" );
	glEdgeFlagPointer           = dllEdgeFlagPointer = GPA( "glEdgeFlagPointer" );
	glEdgeFlagv                 = dllEdgeFlagv = GPA( "glEdgeFlagv" );
	glEnable                    = 	dllEnable                    = GPA( "glEnable" );
	glEnableClientState         = 	dllEnableClientState         = GPA( "glEnableClientState" );
	glEnd                       = 	dllEnd                       = GPA( "glEnd" );
	glEndList                   = 	dllEndList                   = GPA( "glEndList" );
	glEvalCoord1d				 = 	dllEvalCoord1d				 = GPA( "glEvalCoord1d" );
	glEvalCoord1dv              = 	dllEvalCoord1dv              = GPA( "glEvalCoord1dv" );
	glEvalCoord1f               = 	dllEvalCoord1f               = GPA( "glEvalCoord1f" );
	glEvalCoord1fv              = 	dllEvalCoord1fv              = GPA( "glEvalCoord1fv" );
	glEvalCoord2d               = 	dllEvalCoord2d               = GPA( "glEvalCoord2d" );
	glEvalCoord2dv              = 	dllEvalCoord2dv              = GPA( "glEvalCoord2dv" );
	glEvalCoord2f               = 	dllEvalCoord2f               = GPA( "glEvalCoord2f" );
	glEvalCoord2fv              = 	dllEvalCoord2fv              = GPA( "glEvalCoord2fv" );
	glEvalMesh1                 = 	dllEvalMesh1                 = GPA( "glEvalMesh1" );
	glEvalMesh2                 = 	dllEvalMesh2                 = GPA( "glEvalMesh2" );
	glEvalPoint1                = 	dllEvalPoint1                = GPA( "glEvalPoint1" );
	glEvalPoint2                = 	dllEvalPoint2                = GPA( "glEvalPoint2" );
	glFeedbackBuffer            = 	dllFeedbackBuffer            = GPA( "glFeedbackBuffer" );
	glFinish                    = 	dllFinish                    = GPA( "glFinish" );
	glFlush                     = 	dllFlush                     = GPA( "glFlush" );
	glFogf                      = 	dllFogf                      = GPA( "glFogf" );
	glFogfv                     = 	dllFogfv                     = GPA( "glFogfv" );
	glFogi                      = 	dllFogi                      = GPA( "glFogi" );
	glFogiv                     = 	dllFogiv                     = GPA( "glFogiv" );
	glFrontFace                 = 	dllFrontFace                 = GPA( "glFrontFace" );
	glFrustum                   = 	dllFrustum                   = GPA( "glFrustum" );
	glGenLists                  = 	dllGenLists                  = GPA( "glGenLists" );
	glGenTextures               = 	dllGenTextures               = GPA( "glGenTextures" );
	glGetBooleanv               = 	dllGetBooleanv               = GPA( "glGetBooleanv" );
	glGetClipPlane              = 	dllGetClipPlane              = GPA( "glGetClipPlane" );
	glGetDoublev                = 	dllGetDoublev                = GPA( "glGetDoublev" );
	glGetError                  = 	dllGetError                  = GPA( "glGetError" );
	glGetFloatv                 = 	dllGetFloatv                 = GPA( "glGetFloatv" );
	glGetIntegerv               = 	dllGetIntegerv               = GPA( "glGetIntegerv" );
	glGetLightfv                = 	dllGetLightfv                = GPA( "glGetLightfv" );
	glGetLightiv                = 	dllGetLightiv                = GPA( "glGetLightiv" );
	glGetMapdv                  = 	dllGetMapdv                  = GPA( "glGetMapdv" );
	glGetMapfv                  = 	dllGetMapfv                  = GPA( "glGetMapfv" );
	glGetMapiv                  = 	dllGetMapiv                  = GPA( "glGetMapiv" );
	glGetMaterialfv             = 	dllGetMaterialfv             = GPA( "glGetMaterialfv" );
	glGetMaterialiv             = 	dllGetMaterialiv             = GPA( "glGetMaterialiv" );
	glGetPixelMapfv             = 	dllGetPixelMapfv             = GPA( "glGetPixelMapfv" );
	glGetPixelMapuiv            = 	dllGetPixelMapuiv            = GPA( "glGetPixelMapuiv" );
	glGetPixelMapusv            = 	dllGetPixelMapusv            = GPA( "glGetPixelMapusv" );
	glGetPointerv               = 	dllGetPointerv               = GPA( "glGetPointerv" );
	glGetPolygonStipple         = 	dllGetPolygonStipple         = GPA( "glGetPolygonStipple" );
	glGetString                 = 	dllGetString                 = GPA( "glGetString" );
	glGetTexEnvfv               = 	dllGetTexEnvfv               = GPA( "glGetTexEnvfv" );
	glGetTexEnviv               = 	dllGetTexEnviv               = GPA( "glGetTexEnviv" );
	glGetTexGendv               = 	dllGetTexGendv               = GPA( "glGetTexGendv" );
	glGetTexGenfv               = 	dllGetTexGenfv               = GPA( "glGetTexGenfv" );
	glGetTexGeniv               = 	dllGetTexGeniv               = GPA( "glGetTexGeniv" );
	glGetTexImage               = 	dllGetTexImage               = GPA( "glGetTexImage" );
	glGetTexLevelParameterfv    = 	dllGetTexLevelParameterfv    = GPA( "glGetLevelParameterfv" );
	glGetTexLevelParameteriv    = 	dllGetTexLevelParameteriv    = GPA( "glGetLevelParameteriv" );
	glGetTexParameterfv         = 	dllGetTexParameterfv         = GPA( "glGetTexParameterfv" );
	glGetTexParameteriv         = 	dllGetTexParameteriv         = GPA( "glGetTexParameteriv" );
	glHint                      = 	dllHint                      = GPA( "glHint" );
	glIndexMask                 = 	dllIndexMask                 = GPA( "glIndexMask" );
	glIndexPointer              = 	dllIndexPointer              = GPA( "glIndexPointer" );
	glIndexd                    = 	dllIndexd                    = GPA( "glIndexd" );
	glIndexdv                   = 	dllIndexdv                   = GPA( "glIndexdv" );
	glIndexf                    = 	dllIndexf                    = GPA( "glIndexf" );
	glIndexfv                   = 	dllIndexfv                   = GPA( "glIndexfv" );
	glIndexi                    = 	dllIndexi                    = GPA( "glIndexi" );
	glIndexiv                   = 	dllIndexiv                   = GPA( "glIndexiv" );
	glIndexs                    = 	dllIndexs                    = GPA( "glIndexs" );
	glIndexsv                   = 	dllIndexsv                   = GPA( "glIndexsv" );
	glIndexub                   = 	dllIndexub                   = GPA( "glIndexub" );
	glIndexubv                  = 	dllIndexubv                  = GPA( "glIndexubv" );
	glInitNames                 = 	dllInitNames                 = GPA( "glInitNames" );
	glInterleavedArrays         = 	dllInterleavedArrays         = GPA( "glInterleavedArrays" );
	glIsEnabled                 = 	dllIsEnabled                 = GPA( "glIsEnabled" );
	glIsList                    = 	dllIsList                    = GPA( "glIsList" );
	glIsTexture                 = 	dllIsTexture                 = GPA( "glIsTexture" );
	glLightModelf               = 	dllLightModelf               = GPA( "glLightModelf" );
	glLightModelfv              = 	dllLightModelfv              = GPA( "glLightModelfv" );
	glLightModeli               = 	dllLightModeli               = GPA( "glLightModeli" );
	glLightModeliv              = 	dllLightModeliv              = GPA( "glLightModeliv" );
	glLightf                    = 	dllLightf                    = GPA( "glLightf" );
	glLightfv                   = 	dllLightfv                   = GPA( "glLightfv" );
	glLighti                    = 	dllLighti                    = GPA( "glLighti" );
	glLightiv                   = 	dllLightiv                   = GPA( "glLightiv" );
	glLineStipple               = 	dllLineStipple               = GPA( "glLineStipple" );
	glLineWidth                 = 	dllLineWidth                 = GPA( "glLineWidth" );
	glListBase                  = 	dllListBase                  = GPA( "glListBase" );
	glLoadIdentity              = 	dllLoadIdentity              = GPA( "glLoadIdentity" );
	glLoadMatrixd               = 	dllLoadMatrixd               = GPA( "glLoadMatrixd" );
	glLoadMatrixf               = 	dllLoadMatrixf               = GPA( "glLoadMatrixf" );
	glLoadName                  = 	dllLoadName                  = GPA( "glLoadName" );
	glLogicOp                   = 	dllLogicOp                   = GPA( "glLogicOp" );
	glMap1d                     = 	dllMap1d                     = GPA( "glMap1d" );
	glMap1f                     = 	dllMap1f                     = GPA( "glMap1f" );
	glMap2d                     = 	dllMap2d                     = GPA( "glMap2d" );
	glMap2f                     = 	dllMap2f                     = GPA( "glMap2f" );
	glMapGrid1d                 = 	dllMapGrid1d                 = GPA( "glMapGrid1d" );
	glMapGrid1f                 = 	dllMapGrid1f                 = GPA( "glMapGrid1f" );
	glMapGrid2d                 = 	dllMapGrid2d                 = GPA( "glMapGrid2d" );
	glMapGrid2f                 = 	dllMapGrid2f                 = GPA( "glMapGrid2f" );
	glMaterialf                 = 	dllMaterialf                 = GPA( "glMaterialf" );
	glMaterialfv                = 	dllMaterialfv                = GPA( "glMaterialfv" );
	glMateriali                 = 	dllMateriali                 = GPA( "glMateriali" );
	glMaterialiv                = 	dllMaterialiv                = GPA( "glMaterialiv" );
	glMatrixMode                = 	dllMatrixMode                = GPA( "glMatrixMode" );
	glMultMatrixd               = 	dllMultMatrixd               = GPA( "glMultMatrixd" );
	glMultMatrixf               = 	dllMultMatrixf               = GPA( "glMultMatrixf" );
	glNewList                   = 	dllNewList                   = GPA( "glNewList" );
	glNormal3b                  = 	dllNormal3b                  = GPA( "glNormal3b" );
	glNormal3bv                 = 	dllNormal3bv                 = GPA( "glNormal3bv" );
	glNormal3d                  = 	dllNormal3d                  = GPA( "glNormal3d" );
	glNormal3dv                 = 	dllNormal3dv                 = GPA( "glNormal3dv" );
	glNormal3f                  = 	dllNormal3f                  = GPA( "glNormal3f" );
	glNormal3fv                 = 	dllNormal3fv                 = GPA( "glNormal3fv" );
	glNormal3i                  = 	dllNormal3i                  = GPA( "glNormal3i" );
	glNormal3iv                 = 	dllNormal3iv                 = GPA( "glNormal3iv" );
	glNormal3s                  = 	dllNormal3s                  = GPA( "glNormal3s" );
	glNormal3sv                 = 	dllNormal3sv                 = GPA( "glNormal3sv" );
	glNormalPointer             = 	dllNormalPointer             = GPA( "glNormalPointer" );
	glOrtho                     = 	dllOrtho                     = GPA( "glOrtho" );
	glPassThrough               = 	dllPassThrough               = GPA( "glPassThrough" );
	glPixelMapfv                = 	dllPixelMapfv                = GPA( "glPixelMapfv" );
	glPixelMapuiv               = 	dllPixelMapuiv               = GPA( "glPixelMapuiv" );
	glPixelMapusv               = 	dllPixelMapusv               = GPA( "glPixelMapusv" );
	glPixelStoref               = 	dllPixelStoref               = GPA( "glPixelStoref" );
	glPixelStorei               = 	dllPixelStorei               = GPA( "glPixelStorei" );
	glPixelTransferf            = 	dllPixelTransferf            = GPA( "glPixelTransferf" );
	glPixelTransferi            = 	dllPixelTransferi            = GPA( "glPixelTransferi" );
	glPixelZoom                 = 	dllPixelZoom                 = GPA( "glPixelZoom" );
	glPointSize                 = 	dllPointSize                 = GPA( "glPointSize" );
	glPolygonMode               = 	dllPolygonMode               = GPA( "glPolygonMode" );
	glPolygonOffset             = 	dllPolygonOffset             = GPA( "glPolygonOffset" );
	glPolygonStipple            = 	dllPolygonStipple            = GPA( "glPolygonStipple" );
	glPopAttrib                 = 	dllPopAttrib                 = GPA( "glPopAttrib" );
	glPopClientAttrib           = 	dllPopClientAttrib           = GPA( "glPopClientAttrib" );
	glPopMatrix                 = 	dllPopMatrix                 = GPA( "glPopMatrix" );
	glPopName                   = 	dllPopName                   = GPA( "glPopName" );
	glPrioritizeTextures        = 	dllPrioritizeTextures        = GPA( "glPrioritizeTextures" );
	glPushAttrib                = 	dllPushAttrib                = GPA( "glPushAttrib" );
	glPushClientAttrib          = 	dllPushClientAttrib          = GPA( "glPushClientAttrib" );
	glPushMatrix                = 	dllPushMatrix                = GPA( "glPushMatrix" );
	glPushName                  = 	dllPushName                  = GPA( "glPushName" );
	glRasterPos2d               = 	dllRasterPos2d               = GPA( "glRasterPos2d" );
	glRasterPos2dv              = 	dllRasterPos2dv              = GPA( "glRasterPos2dv" );
	glRasterPos2f               = 	dllRasterPos2f               = GPA( "glRasterPos2f" );
	glRasterPos2fv              = 	dllRasterPos2fv              = GPA( "glRasterPos2fv" );
	glRasterPos2i               = 	dllRasterPos2i               = GPA( "glRasterPos2i" );
	glRasterPos2iv              = 	dllRasterPos2iv              = GPA( "glRasterPos2iv" );
	glRasterPos2s               = 	dllRasterPos2s               = GPA( "glRasterPos2s" );
	glRasterPos2sv              = 	dllRasterPos2sv              = GPA( "glRasterPos2sv" );
	glRasterPos3d               = 	dllRasterPos3d               = GPA( "glRasterPos3d" );
	glRasterPos3dv              = 	dllRasterPos3dv              = GPA( "glRasterPos3dv" );
	glRasterPos3f               = 	dllRasterPos3f               = GPA( "glRasterPos3f" );
	glRasterPos3fv              = 	dllRasterPos3fv              = GPA( "glRasterPos3fv" );
	glRasterPos3i               = 	dllRasterPos3i               = GPA( "glRasterPos3i" );
	glRasterPos3iv              = 	dllRasterPos3iv              = GPA( "glRasterPos3iv" );
	glRasterPos3s               = 	dllRasterPos3s               = GPA( "glRasterPos3s" );
	glRasterPos3sv              = 	dllRasterPos3sv              = GPA( "glRasterPos3sv" );
	glRasterPos4d               = 	dllRasterPos4d               = GPA( "glRasterPos4d" );
	glRasterPos4dv              = 	dllRasterPos4dv              = GPA( "glRasterPos4dv" );
	glRasterPos4f               = 	dllRasterPos4f               = GPA( "glRasterPos4f" );
	glRasterPos4fv              = 	dllRasterPos4fv              = GPA( "glRasterPos4fv" );
	glRasterPos4i               = 	dllRasterPos4i               = GPA( "glRasterPos4i" );
	glRasterPos4iv              = 	dllRasterPos4iv              = GPA( "glRasterPos4iv" );
	glRasterPos4s               = 	dllRasterPos4s               = GPA( "glRasterPos4s" );
	glRasterPos4sv              = 	dllRasterPos4sv              = GPA( "glRasterPos4sv" );
	glReadBuffer                = 	dllReadBuffer                = GPA( "glReadBuffer" );
	glReadPixels                = 	dllReadPixels                = GPA( "glReadPixels" );
	glRectd                     = 	dllRectd                     = GPA( "glRectd" );
	glRectdv                    = 	dllRectdv                    = GPA( "glRectdv" );
	glRectf                     = 	dllRectf                     = GPA( "glRectf" );
	glRectfv                    = 	dllRectfv                    = GPA( "glRectfv" );
	glRecti                     = 	dllRecti                     = GPA( "glRecti" );
	glRectiv                    = 	dllRectiv                    = GPA( "glRectiv" );
	glRects                     = 	dllRects                     = GPA( "glRects" );
	glRectsv                    = 	dllRectsv                    = GPA( "glRectsv" );
	glRenderMode                = 	dllRenderMode                = GPA( "glRenderMode" );
	glRotated                   = 	dllRotated                   = GPA( "glRotated" );
	glRotatef                   = 	dllRotatef                   = GPA( "glRotatef" );
	glScaled                    = 	dllScaled                    = GPA( "glScaled" );
	glScalef                    = 	dllScalef                    = GPA( "glScalef" );
	glScissor                   = 	dllScissor                   = GPA( "glScissor" );
	glSelectBuffer              = 	dllSelectBuffer              = GPA( "glSelectBuffer" );
	glShadeModel                = 	dllShadeModel                = GPA( "glShadeModel" );
	glStencilFunc               = 	dllStencilFunc               = GPA( "glStencilFunc" );
	glStencilMask               = 	dllStencilMask               = GPA( "glStencilMask" );
	glStencilOp                 = 	dllStencilOp                 = GPA( "glStencilOp" );
	glTexCoord1d                = 	dllTexCoord1d                = GPA( "glTexCoord1d" );
	glTexCoord1dv               = 	dllTexCoord1dv               = GPA( "glTexCoord1dv" );
	glTexCoord1f                = 	dllTexCoord1f                = GPA( "glTexCoord1f" );
	glTexCoord1fv               = 	dllTexCoord1fv               = GPA( "glTexCoord1fv" );
	glTexCoord1i                = 	dllTexCoord1i                = GPA( "glTexCoord1i" );
	glTexCoord1iv               = 	dllTexCoord1iv               = GPA( "glTexCoord1iv" );
	glTexCoord1s                = 	dllTexCoord1s                = GPA( "glTexCoord1s" );
	glTexCoord1sv               = 	dllTexCoord1sv               = GPA( "glTexCoord1sv" );
	glTexCoord2d                = 	dllTexCoord2d                = GPA( "glTexCoord2d" );
	glTexCoord2dv               = 	dllTexCoord2dv               = GPA( "glTexCoord2dv" );
	glTexCoord2f                = 	dllTexCoord2f                = GPA( "glTexCoord2f" );
	glTexCoord2fv               = 	dllTexCoord2fv               = GPA( "glTexCoord2fv" );
	glTexCoord2i                = 	dllTexCoord2i                = GPA( "glTexCoord2i" );
	glTexCoord2iv               = 	dllTexCoord2iv               = GPA( "glTexCoord2iv" );
	glTexCoord2s                = 	dllTexCoord2s                = GPA( "glTexCoord2s" );
	glTexCoord2sv               = 	dllTexCoord2sv               = GPA( "glTexCoord2sv" );
	glTexCoord3d                = 	dllTexCoord3d                = GPA( "glTexCoord3d" );
	glTexCoord3dv               = 	dllTexCoord3dv               = GPA( "glTexCoord3dv" );
	glTexCoord3f                = 	dllTexCoord3f                = GPA( "glTexCoord3f" );
	glTexCoord3fv               = 	dllTexCoord3fv               = GPA( "glTexCoord3fv" );
	glTexCoord3i                = 	dllTexCoord3i                = GPA( "glTexCoord3i" );
	glTexCoord3iv               = 	dllTexCoord3iv               = GPA( "glTexCoord3iv" );
	glTexCoord3s                = 	dllTexCoord3s                = GPA( "glTexCoord3s" );
	glTexCoord3sv               = 	dllTexCoord3sv               = GPA( "glTexCoord3sv" );
	glTexCoord4d                = 	dllTexCoord4d                = GPA( "glTexCoord4d" );
	glTexCoord4dv               = 	dllTexCoord4dv               = GPA( "glTexCoord4dv" );
	glTexCoord4f                = 	dllTexCoord4f                = GPA( "glTexCoord4f" );
	glTexCoord4fv               = 	dllTexCoord4fv               = GPA( "glTexCoord4fv" );
	glTexCoord4i                = 	dllTexCoord4i                = GPA( "glTexCoord4i" );
	glTexCoord4iv               = 	dllTexCoord4iv               = GPA( "glTexCoord4iv" );
	glTexCoord4s                = 	dllTexCoord4s                = GPA( "glTexCoord4s" );
	glTexCoord4sv               = 	dllTexCoord4sv               = GPA( "glTexCoord4sv" );
	glTexCoordPointer           = 	dllTexCoordPointer           = GPA( "glTexCoordPointer" );
	glTexEnvf                   = 	dllTexEnvf                   = GPA( "glTexEnvf" );
	glTexEnvfv                  = 	dllTexEnvfv                  = GPA( "glTexEnvfv" );
	glTexEnvi                   = 	dllTexEnvi                   = GPA( "glTexEnvi" );
	glTexEnviv                  = 	dllTexEnviv                  = GPA( "glTexEnviv" );
	glTexGend                   = 	dllTexGend                   = GPA( "glTexGend" );
	glTexGendv                  = 	dllTexGendv                  = GPA( "glTexGendv" );
	glTexGenf                   = 	dllTexGenf                   = GPA( "glTexGenf" );
	glTexGenfv                  = 	dllTexGenfv                  = GPA( "glTexGenfv" );
	glTexGeni                   = 	dllTexGeni                   = GPA( "glTexGeni" );
	glTexGeniv                  = 	dllTexGeniv                  = GPA( "glTexGeniv" );
	glTexImage1D                = 	dllTexImage1D                = GPA( "glTexImage1D" );
	glTexImage2D                = 	dllTexImage2D                = GPA( "glTexImage2D" );
	glTexParameterf             = 	dllTexParameterf             = GPA( "glTexParameterf" );
	glTexParameterfv            = 	dllTexParameterfv            = GPA( "glTexParameterfv" );
	glTexParameteri             = 	dllTexParameteri             = GPA( "glTexParameteri" );
	glTexParameteriv            = 	dllTexParameteriv            = GPA( "glTexParameteriv" );
	glTexSubImage1D             = 	dllTexSubImage1D             = GPA( "glTexSubImage1D" );
	glTexSubImage2D             = 	dllTexSubImage2D             = GPA( "glTexSubImage2D" );
	glTranslated                = 	dllTranslated                = GPA( "glTranslated" );
	glTranslatef                = 	dllTranslatef                = GPA( "glTranslatef" );
	glVertex2d                  = 	dllVertex2d                  = GPA( "glVertex2d" );
	glVertex2dv                 = 	dllVertex2dv                 = GPA( "glVertex2dv" );
	glVertex2f                  = 	dllVertex2f                  = GPA( "glVertex2f" );
	glVertex2fv                 = 	dllVertex2fv                 = GPA( "glVertex2fv" );
	glVertex2i                  = 	dllVertex2i                  = GPA( "glVertex2i" );
	glVertex2iv                 = 	dllVertex2iv                 = GPA( "glVertex2iv" );
	glVertex2s                  = 	dllVertex2s                  = GPA( "glVertex2s" );
	glVertex2sv                 = 	dllVertex2sv                 = GPA( "glVertex2sv" );
	glVertex3d                  = 	dllVertex3d                  = GPA( "glVertex3d" );
	glVertex3dv                 = 	dllVertex3dv                 = GPA( "glVertex3dv" );
	glVertex3f                  = 	dllVertex3f                  = GPA( "glVertex3f" );
	glVertex3fv                 = 	dllVertex3fv                 = GPA( "glVertex3fv" );
	glVertex3i                  = 	dllVertex3i                  = GPA( "glVertex3i" );
	glVertex3iv                 = 	dllVertex3iv                 = GPA( "glVertex3iv" );
	glVertex3s                  = 	dllVertex3s                  = GPA( "glVertex3s" );
	glVertex3sv                 = 	dllVertex3sv                 = GPA( "glVertex3sv" );
	glVertex4d                  = 	dllVertex4d                  = GPA( "glVertex4d" );
	glVertex4dv                 = 	dllVertex4dv                 = GPA( "glVertex4dv" );
	glVertex4f                  = 	dllVertex4f                  = GPA( "glVertex4f" );
	glVertex4fv                 = 	dllVertex4fv                 = GPA( "glVertex4fv" );
	glVertex4i                  = 	dllVertex4i                  = GPA( "glVertex4i" );
	glVertex4iv                 = 	dllVertex4iv                 = GPA( "glVertex4iv" );
	glVertex4s                  = 	dllVertex4s                  = GPA( "glVertex4s" );
	glVertex4sv                 = 	dllVertex4sv                 = GPA( "glVertex4sv" );
	glVertexPointer             = 	dllVertexPointer             = GPA( "glVertexPointer" );
	glViewport                  = 	dllViewport                  = GPA( "glViewport" );

	qwglCopyContext              = GPA( "wglCopyContext" );
	qwglCreateContext            = GPA( "wglCreateContext" );
	qwglCreateLayerContext       = GPA( "wglCreateLayerContext" );
	qwglDeleteContext            = GPA( "wglDeleteContext" );
	qwglDescribeLayerPlane       = GPA( "wglDescribeLayerPlane" );
	qwglGetCurrentContext        = GPA( "wglGetCurrentContext" );
	qwglGetCurrentDC             = GPA( "wglGetCurrentDC" );
	qwglGetLayerPaletteEntries   = GPA( "wglGetLayerPaletteEntries" );
	qwglGetProcAddress           = GPA( "wglGetProcAddress" );
	qwglMakeCurrent              = GPA( "wglMakeCurrent" );
	qwglRealizeLayerPalette      = GPA( "wglRealizeLayerPalette" );
	qwglSetLayerPaletteEntries   = GPA( "wglSetLayerPaletteEntries" );
	qwglShareLists               = GPA( "wglShareLists" );
	qwglSwapLayerBuffers         = GPA( "wglSwapLayerBuffers" );
	qwglUseFontBitmaps           = GPA( "wglUseFontBitmapsA" );
	qwglUseFontOutlines          = GPA( "wglUseFontOutlinesA" );

	qwglChoosePixelFormat        = GPA( "wglChoosePixelFormat" );
	qwglDescribePixelFormat      = GPA( "wglDescribePixelFormat" );
	qwglGetPixelFormat           = GPA( "wglGetPixelFormat" );
	qwglSetPixelFormat           = GPA( "wglSetPixelFormat" );
	qwglSwapBuffers              = GPA( "wglSwapBuffers" );

	qwglSwapIntervalEXT = 0;
	glPointParameterfEXT = 0;
	glPointParameterfvEXT = 0;
	glColorTableEXT = 0;
	glSelectTextureSGIS = 0;
	glMultiTexCoord2f = 0;

	return true;
}

void GLimp_EnableLogging( qboolean enable )
{
	if ( enable )
	{
		if ( !glw_state.log_fp )
		{
			struct tm *newtime;
			time_t aclock;
			char buffer[1024];

			time( &aclock );
			newtime = localtime( &aclock );

			asctime( newtime );

			Com_sprintf( buffer, sizeof(buffer), "%s/gl.log", ri.FS_Gamedir() ); 
			glw_state.log_fp = fopen( buffer, "wt" );

			fprintf( glw_state.log_fp, "%s\n", asctime( newtime ) );
		}

		glAccum                     = logAccum;
		glAlphaFunc                 = logAlphaFunc;
		glAreTexturesResident       = logAreTexturesResident;
		glArrayElement              = logArrayElement;
		glBegin                     = logBegin;
		glBindTexture               = logBindTexture;
		glBitmap                    = logBitmap;
		glBlendFunc                 = logBlendFunc;
		glCallList                  = logCallList;
		glCallLists                 = logCallLists;
		glClear                     = logClear;
		glClearAccum                = logClearAccum;
		glClearColor                = logClearColor;
		glClearDepth                = logClearDepth;
		glClearIndex                = logClearIndex;
		glClearStencil              = logClearStencil;
		glClipPlane                 = logClipPlane;
		glColor3b                   = logColor3b;
		glColor3bv                  = logColor3bv;
		glColor3d                   = logColor3d;
		glColor3dv                  = logColor3dv;
		glColor3f                   = logColor3f;
		glColor3fv                  = logColor3fv;
		glColor3i                   = logColor3i;
		glColor3iv                  = logColor3iv;
		glColor3s                   = logColor3s;
		glColor3sv                  = logColor3sv;
		glColor3ub                  = logColor3ub;
		glColor3ubv                 = logColor3ubv;
		glColor3ui                  = logColor3ui;
		glColor3uiv                 = logColor3uiv;
		glColor3us                  = logColor3us;
		glColor3usv                 = logColor3usv;
		glColor4b                   = logColor4b;
		glColor4bv                  = logColor4bv;
		glColor4d                   = logColor4d;
		glColor4dv                  = logColor4dv;
		glColor4f                   = logColor4f;
		glColor4fv                  = logColor4fv;
		glColor4i                   = logColor4i;
		glColor4iv                  = logColor4iv;
		glColor4s                   = logColor4s;
		glColor4sv                  = logColor4sv;
		glColor4ub                  = logColor4ub;
		glColor4ubv                 = logColor4ubv;
		glColor4ui                  = logColor4ui;
		glColor4uiv                 = logColor4uiv;
		glColor4us                  = logColor4us;
		glColor4usv                 = logColor4usv;
		glColorMask                 = logColorMask;
		glColorMaterial             = logColorMaterial;
		glColorPointer              = logColorPointer;
		glCopyPixels                = logCopyPixels;
		glCopyTexImage1D            = logCopyTexImage1D;
		glCopyTexImage2D            = logCopyTexImage2D;
		glCopyTexSubImage1D         = logCopyTexSubImage1D;
		glCopyTexSubImage2D         = logCopyTexSubImage2D;
		glCullFace                  = logCullFace;
		glDeleteLists               = logDeleteLists ;
		glDeleteTextures            = logDeleteTextures ;
		glDepthFunc                 = logDepthFunc ;
		glDepthMask                 = logDepthMask ;
		glDepthRange                = logDepthRange ;
		glDisable                   = logDisable ;
		glDisableClientState        = logDisableClientState ;
		glDrawArrays                = logDrawArrays ;
		glDrawBuffer                = logDrawBuffer ;
		glDrawElements              = logDrawElements ;
		glDrawPixels                = logDrawPixels ;
		glEdgeFlag                  = logEdgeFlag ;
		glEdgeFlagPointer           = logEdgeFlagPointer ;
		glEdgeFlagv                 = logEdgeFlagv ;
		glEnable                    = 	logEnable                    ;
		glEnableClientState         = 	logEnableClientState         ;
		glEnd                       = 	logEnd                       ;
		glEndList                   = 	logEndList                   ;
		glEvalCoord1d				 = 	logEvalCoord1d				 ;
		glEvalCoord1dv              = 	logEvalCoord1dv              ;
		glEvalCoord1f               = 	logEvalCoord1f               ;
		glEvalCoord1fv              = 	logEvalCoord1fv              ;
		glEvalCoord2d               = 	logEvalCoord2d               ;
		glEvalCoord2dv              = 	logEvalCoord2dv              ;
		glEvalCoord2f               = 	logEvalCoord2f               ;
		glEvalCoord2fv              = 	logEvalCoord2fv              ;
		glEvalMesh1                 = 	logEvalMesh1                 ;
		glEvalMesh2                 = 	logEvalMesh2                 ;
		glEvalPoint1                = 	logEvalPoint1                ;
		glEvalPoint2                = 	logEvalPoint2                ;
		glFeedbackBuffer            = 	logFeedbackBuffer            ;
		glFinish                    = 	logFinish                    ;
		glFlush                     = 	logFlush                     ;
		glFogf                      = 	logFogf                      ;
		glFogfv                     = 	logFogfv                     ;
		glFogi                      = 	logFogi                      ;
		glFogiv                     = 	logFogiv                     ;
		glFrontFace                 = 	logFrontFace                 ;
		glFrustum                   = 	logFrustum                   ;
		glGenLists                  = 	logGenLists                  ;
		glGenTextures               = 	logGenTextures               ;
		glGetBooleanv               = 	logGetBooleanv               ;
		glGetClipPlane              = 	logGetClipPlane              ;
		glGetDoublev                = 	logGetDoublev                ;
		glGetError                  = 	logGetError                  ;
		glGetFloatv                 = 	logGetFloatv                 ;
		glGetIntegerv               = 	logGetIntegerv               ;
		glGetLightfv                = 	logGetLightfv                ;
		glGetLightiv                = 	logGetLightiv                ;
		glGetMapdv                  = 	logGetMapdv                  ;
		glGetMapfv                  = 	logGetMapfv                  ;
		glGetMapiv                  = 	logGetMapiv                  ;
		glGetMaterialfv             = 	logGetMaterialfv             ;
		glGetMaterialiv             = 	logGetMaterialiv             ;
		glGetPixelMapfv             = 	logGetPixelMapfv             ;
		glGetPixelMapuiv            = 	logGetPixelMapuiv            ;
		glGetPixelMapusv            = 	logGetPixelMapusv            ;
		glGetPointerv               = 	logGetPointerv               ;
		glGetPolygonStipple         = 	logGetPolygonStipple         ;
		glGetString                 = 	logGetString                 ;
		glGetTexEnvfv               = 	logGetTexEnvfv               ;
		glGetTexEnviv               = 	logGetTexEnviv               ;
		glGetTexGendv               = 	logGetTexGendv               ;
		glGetTexGenfv               = 	logGetTexGenfv               ;
		glGetTexGeniv               = 	logGetTexGeniv               ;
		glGetTexImage               = 	logGetTexImage               ;
		glGetTexLevelParameterfv    = 	logGetTexLevelParameterfv    ;
		glGetTexLevelParameteriv    = 	logGetTexLevelParameteriv    ;
		glGetTexParameterfv         = 	logGetTexParameterfv         ;
		glGetTexParameteriv         = 	logGetTexParameteriv         ;
		glHint                      = 	logHint                      ;
		glIndexMask                 = 	logIndexMask                 ;
		glIndexPointer              = 	logIndexPointer              ;
		glIndexd                    = 	logIndexd                    ;
		glIndexdv                   = 	logIndexdv                   ;
		glIndexf                    = 	logIndexf                    ;
		glIndexfv                   = 	logIndexfv                   ;
		glIndexi                    = 	logIndexi                    ;
		glIndexiv                   = 	logIndexiv                   ;
		glIndexs                    = 	logIndexs                    ;
		glIndexsv                   = 	logIndexsv                   ;
		glIndexub                   = 	logIndexub                   ;
		glIndexubv                  = 	logIndexubv                  ;
		glInitNames                 = 	logInitNames                 ;
		glInterleavedArrays         = 	logInterleavedArrays         ;
		glIsEnabled                 = 	logIsEnabled                 ;
		glIsList                    = 	logIsList                    ;
		glIsTexture                 = 	logIsTexture                 ;
		glLightModelf               = 	logLightModelf               ;
		glLightModelfv              = 	logLightModelfv              ;
		glLightModeli               = 	logLightModeli               ;
		glLightModeliv              = 	logLightModeliv              ;
		glLightf                    = 	logLightf                    ;
		glLightfv                   = 	logLightfv                   ;
		glLighti                    = 	logLighti                    ;
		glLightiv                   = 	logLightiv                   ;
		glLineStipple               = 	logLineStipple               ;
		glLineWidth                 = 	logLineWidth                 ;
		glListBase                  = 	logListBase                  ;
		glLoadIdentity              = 	logLoadIdentity              ;
		glLoadMatrixd               = 	logLoadMatrixd               ;
		glLoadMatrixf               = 	logLoadMatrixf               ;
		glLoadName                  = 	logLoadName                  ;
		glLogicOp                   = 	logLogicOp                   ;
		glMap1d                     = 	logMap1d                     ;
		glMap1f                     = 	logMap1f                     ;
		glMap2d                     = 	logMap2d                     ;
		glMap2f                     = 	logMap2f                     ;
		glMapGrid1d                 = 	logMapGrid1d                 ;
		glMapGrid1f                 = 	logMapGrid1f                 ;
		glMapGrid2d                 = 	logMapGrid2d                 ;
		glMapGrid2f                 = 	logMapGrid2f                 ;
		glMaterialf                 = 	logMaterialf                 ;
		glMaterialfv                = 	logMaterialfv                ;
		glMateriali                 = 	logMateriali                 ;
		glMaterialiv                = 	logMaterialiv                ;
		glMatrixMode                = 	logMatrixMode                ;
		glMultMatrixd               = 	logMultMatrixd               ;
		glMultMatrixf               = 	logMultMatrixf               ;
		glNewList                   = 	logNewList                   ;
		glNormal3b                  = 	logNormal3b                  ;
		glNormal3bv                 = 	logNormal3bv                 ;
		glNormal3d                  = 	logNormal3d                  ;
		glNormal3dv                 = 	logNormal3dv                 ;
		glNormal3f                  = 	logNormal3f                  ;
		glNormal3fv                 = 	logNormal3fv                 ;
		glNormal3i                  = 	logNormal3i                  ;
		glNormal3iv                 = 	logNormal3iv                 ;
		glNormal3s                  = 	logNormal3s                  ;
		glNormal3sv                 = 	logNormal3sv                 ;
		glNormalPointer             = 	logNormalPointer             ;
		glOrtho                     = 	logOrtho                     ;
		glPassThrough               = 	logPassThrough               ;
		glPixelMapfv                = 	logPixelMapfv                ;
		glPixelMapuiv               = 	logPixelMapuiv               ;
		glPixelMapusv               = 	logPixelMapusv               ;
		glPixelStoref               = 	logPixelStoref               ;
		glPixelStorei               = 	logPixelStorei               ;
		glPixelTransferf            = 	logPixelTransferf            ;
		glPixelTransferi            = 	logPixelTransferi            ;
		glPixelZoom                 = 	logPixelZoom                 ;
		glPointSize                 = 	logPointSize                 ;
		glPolygonMode               = 	logPolygonMode               ;
		glPolygonOffset             = 	logPolygonOffset             ;
		glPolygonStipple            = 	logPolygonStipple            ;
		glPopAttrib                 = 	logPopAttrib                 ;
		glPopClientAttrib           = 	logPopClientAttrib           ;
		glPopMatrix                 = 	logPopMatrix                 ;
		glPopName                   = 	logPopName                   ;
		glPrioritizeTextures        = 	logPrioritizeTextures        ;
		glPushAttrib                = 	logPushAttrib                ;
		glPushClientAttrib          = 	logPushClientAttrib          ;
		glPushMatrix                = 	logPushMatrix                ;
		glPushName                  = 	logPushName                  ;
		glRasterPos2d               = 	logRasterPos2d               ;
		glRasterPos2dv              = 	logRasterPos2dv              ;
		glRasterPos2f               = 	logRasterPos2f               ;
		glRasterPos2fv              = 	logRasterPos2fv              ;
		glRasterPos2i               = 	logRasterPos2i               ;
		glRasterPos2iv              = 	logRasterPos2iv              ;
		glRasterPos2s               = 	logRasterPos2s               ;
		glRasterPos2sv              = 	logRasterPos2sv              ;
		glRasterPos3d               = 	logRasterPos3d               ;
		glRasterPos3dv              = 	logRasterPos3dv              ;
		glRasterPos3f               = 	logRasterPos3f               ;
		glRasterPos3fv              = 	logRasterPos3fv              ;
		glRasterPos3i               = 	logRasterPos3i               ;
		glRasterPos3iv              = 	logRasterPos3iv              ;
		glRasterPos3s               = 	logRasterPos3s               ;
		glRasterPos3sv              = 	logRasterPos3sv              ;
		glRasterPos4d               = 	logRasterPos4d               ;
		glRasterPos4dv              = 	logRasterPos4dv              ;
		glRasterPos4f               = 	logRasterPos4f               ;
		glRasterPos4fv              = 	logRasterPos4fv              ;
		glRasterPos4i               = 	logRasterPos4i               ;
		glRasterPos4iv              = 	logRasterPos4iv              ;
		glRasterPos4s               = 	logRasterPos4s               ;
		glRasterPos4sv              = 	logRasterPos4sv              ;
		glReadBuffer                = 	logReadBuffer                ;
		glReadPixels                = 	logReadPixels                ;
		glRectd                     = 	logRectd                     ;
		glRectdv                    = 	logRectdv                    ;
		glRectf                     = 	logRectf                     ;
		glRectfv                    = 	logRectfv                    ;
		glRecti                     = 	logRecti                     ;
		glRectiv                    = 	logRectiv                    ;
		glRects                     = 	logRects                     ;
		glRectsv                    = 	logRectsv                    ;
		glRenderMode                = 	logRenderMode                ;
		glRotated                   = 	logRotated                   ;
		glRotatef                   = 	logRotatef                   ;
		glScaled                    = 	logScaled                    ;
		glScalef                    = 	logScalef                    ;
		glScissor                   = 	logScissor                   ;
		glSelectBuffer              = 	logSelectBuffer              ;
		glShadeModel                = 	logShadeModel                ;
		glStencilFunc               = 	logStencilFunc               ;
		glStencilMask               = 	logStencilMask               ;
		glStencilOp                 = 	logStencilOp                 ;
		glTexCoord1d                = 	logTexCoord1d                ;
		glTexCoord1dv               = 	logTexCoord1dv               ;
		glTexCoord1f                = 	logTexCoord1f                ;
		glTexCoord1fv               = 	logTexCoord1fv               ;
		glTexCoord1i                = 	logTexCoord1i                ;
		glTexCoord1iv               = 	logTexCoord1iv               ;
		glTexCoord1s                = 	logTexCoord1s                ;
		glTexCoord1sv               = 	logTexCoord1sv               ;
		glTexCoord2d                = 	logTexCoord2d                ;
		glTexCoord2dv               = 	logTexCoord2dv               ;
		glTexCoord2f                = 	logTexCoord2f                ;
		glTexCoord2fv               = 	logTexCoord2fv               ;
		glTexCoord2i                = 	logTexCoord2i                ;
		glTexCoord2iv               = 	logTexCoord2iv               ;
		glTexCoord2s                = 	logTexCoord2s                ;
		glTexCoord2sv               = 	logTexCoord2sv               ;
		glTexCoord3d                = 	logTexCoord3d                ;
		glTexCoord3dv               = 	logTexCoord3dv               ;
		glTexCoord3f                = 	logTexCoord3f                ;
		glTexCoord3fv               = 	logTexCoord3fv               ;
		glTexCoord3i                = 	logTexCoord3i                ;
		glTexCoord3iv               = 	logTexCoord3iv               ;
		glTexCoord3s                = 	logTexCoord3s                ;
		glTexCoord3sv               = 	logTexCoord3sv               ;
		glTexCoord4d                = 	logTexCoord4d                ;
		glTexCoord4dv               = 	logTexCoord4dv               ;
		glTexCoord4f                = 	logTexCoord4f                ;
		glTexCoord4fv               = 	logTexCoord4fv               ;
		glTexCoord4i                = 	logTexCoord4i                ;
		glTexCoord4iv               = 	logTexCoord4iv               ;
		glTexCoord4s                = 	logTexCoord4s                ;
		glTexCoord4sv               = 	logTexCoord4sv               ;
		glTexCoordPointer           = 	logTexCoordPointer           ;
		glTexEnvf                   = 	logTexEnvf                   ;
		glTexEnvfv                  = 	logTexEnvfv                  ;
		glTexEnvi                   = 	logTexEnvi                   ;
		glTexEnviv                  = 	logTexEnviv                  ;
		glTexGend                   = 	logTexGend                   ;
		glTexGendv                  = 	logTexGendv                  ;
		glTexGenf                   = 	logTexGenf                   ;
		glTexGenfv                  = 	logTexGenfv                  ;
		glTexGeni                   = 	logTexGeni                   ;
		glTexGeniv                  = 	logTexGeniv                  ;
		glTexImage1D                = 	logTexImage1D                ;
		glTexImage2D                = 	logTexImage2D                ;
		glTexParameterf             = 	logTexParameterf             ;
		glTexParameterfv            = 	logTexParameterfv            ;
		glTexParameteri             = 	logTexParameteri             ;
		glTexParameteriv            = 	logTexParameteriv            ;
		glTexSubImage1D             = 	logTexSubImage1D             ;
		glTexSubImage2D             = 	logTexSubImage2D             ;
		glTranslated                = 	logTranslated                ;
		glTranslatef                = 	logTranslatef                ;
		glVertex2d                  = 	logVertex2d                  ;
		glVertex2dv                 = 	logVertex2dv                 ;
		glVertex2f                  = 	logVertex2f                  ;
		glVertex2fv                 = 	logVertex2fv                 ;
		glVertex2i                  = 	logVertex2i                  ;
		glVertex2iv                 = 	logVertex2iv                 ;
		glVertex2s                  = 	logVertex2s                  ;
		glVertex2sv                 = 	logVertex2sv                 ;
		glVertex3d                  = 	logVertex3d                  ;
		glVertex3dv                 = 	logVertex3dv                 ;
		glVertex3f                  = 	logVertex3f                  ;
		glVertex3fv                 = 	logVertex3fv                 ;
		glVertex3i                  = 	logVertex3i                  ;
		glVertex3iv                 = 	logVertex3iv                 ;
		glVertex3s                  = 	logVertex3s                  ;
		glVertex3sv                 = 	logVertex3sv                 ;
		glVertex4d                  = 	logVertex4d                  ;
		glVertex4dv                 = 	logVertex4dv                 ;
		glVertex4f                  = 	logVertex4f                  ;
		glVertex4fv                 = 	logVertex4fv                 ;
		glVertex4i                  = 	logVertex4i                  ;
		glVertex4iv                 = 	logVertex4iv                 ;
		glVertex4s                  = 	logVertex4s                  ;
		glVertex4sv                 = 	logVertex4sv                 ;
		glVertexPointer             = 	logVertexPointer             ;
		glViewport                  = 	logViewport                  ;
	}
	else
	{
		glAccum                     = dllAccum;
		glAlphaFunc                 = dllAlphaFunc;
		glAreTexturesResident       = dllAreTexturesResident;
		glArrayElement              = dllArrayElement;
		glBegin                     = dllBegin;
		glBindTexture               = dllBindTexture;
		glBitmap                    = dllBitmap;
		glBlendFunc                 = dllBlendFunc;
		glCallList                  = dllCallList;
		glCallLists                 = dllCallLists;
		glClear                     = dllClear;
		glClearAccum                = dllClearAccum;
		glClearColor                = dllClearColor;
		glClearDepth                = dllClearDepth;
		glClearIndex                = dllClearIndex;
		glClearStencil              = dllClearStencil;
		glClipPlane                 = dllClipPlane;
		glColor3b                   = dllColor3b;
		glColor3bv                  = dllColor3bv;
		glColor3d                   = dllColor3d;
		glColor3dv                  = dllColor3dv;
		glColor3f                   = dllColor3f;
		glColor3fv                  = dllColor3fv;
		glColor3i                   = dllColor3i;
		glColor3iv                  = dllColor3iv;
		glColor3s                   = dllColor3s;
		glColor3sv                  = dllColor3sv;
		glColor3ub                  = dllColor3ub;
		glColor3ubv                 = dllColor3ubv;
		glColor3ui                  = dllColor3ui;
		glColor3uiv                 = dllColor3uiv;
		glColor3us                  = dllColor3us;
		glColor3usv                 = dllColor3usv;
		glColor4b                   = dllColor4b;
		glColor4bv                  = dllColor4bv;
		glColor4d                   = dllColor4d;
		glColor4dv                  = dllColor4dv;
		glColor4f                   = dllColor4f;
		glColor4fv                  = dllColor4fv;
		glColor4i                   = dllColor4i;
		glColor4iv                  = dllColor4iv;
		glColor4s                   = dllColor4s;
		glColor4sv                  = dllColor4sv;
		glColor4ub                  = dllColor4ub;
		glColor4ubv                 = dllColor4ubv;
		glColor4ui                  = dllColor4ui;
		glColor4uiv                 = dllColor4uiv;
		glColor4us                  = dllColor4us;
		glColor4usv                 = dllColor4usv;
		glColorMask                 = dllColorMask;
		glColorMaterial             = dllColorMaterial;
		glColorPointer              = dllColorPointer;
		glCopyPixels                = dllCopyPixels;
		glCopyTexImage1D            = dllCopyTexImage1D;
		glCopyTexImage2D            = dllCopyTexImage2D;
		glCopyTexSubImage1D         = dllCopyTexSubImage1D;
		glCopyTexSubImage2D         = dllCopyTexSubImage2D;
		glCullFace                  = dllCullFace;
		glDeleteLists               = dllDeleteLists ;
		glDeleteTextures            = dllDeleteTextures ;
		glDepthFunc                 = dllDepthFunc ;
		glDepthMask                 = dllDepthMask ;
		glDepthRange                = dllDepthRange ;
		glDisable                   = dllDisable ;
		glDisableClientState        = dllDisableClientState ;
		glDrawArrays                = dllDrawArrays ;
		glDrawBuffer                = dllDrawBuffer ;
		glDrawElements              = dllDrawElements ;
		glDrawPixels                = dllDrawPixels ;
		glEdgeFlag                  = dllEdgeFlag ;
		glEdgeFlagPointer           = dllEdgeFlagPointer ;
		glEdgeFlagv                 = dllEdgeFlagv ;
		glEnable                    = 	dllEnable                    ;
		glEnableClientState         = 	dllEnableClientState         ;
		glEnd                       = 	dllEnd                       ;
		glEndList                   = 	dllEndList                   ;
		glEvalCoord1d				 = 	dllEvalCoord1d				 ;
		glEvalCoord1dv              = 	dllEvalCoord1dv              ;
		glEvalCoord1f               = 	dllEvalCoord1f               ;
		glEvalCoord1fv              = 	dllEvalCoord1fv              ;
		glEvalCoord2d               = 	dllEvalCoord2d               ;
		glEvalCoord2dv              = 	dllEvalCoord2dv              ;
		glEvalCoord2f               = 	dllEvalCoord2f               ;
		glEvalCoord2fv              = 	dllEvalCoord2fv              ;
		glEvalMesh1                 = 	dllEvalMesh1                 ;
		glEvalMesh2                 = 	dllEvalMesh2                 ;
		glEvalPoint1                = 	dllEvalPoint1                ;
		glEvalPoint2                = 	dllEvalPoint2                ;
		glFeedbackBuffer            = 	dllFeedbackBuffer            ;
		glFinish                    = 	dllFinish                    ;
		glFlush                     = 	dllFlush                     ;
		glFogf                      = 	dllFogf                      ;
		glFogfv                     = 	dllFogfv                     ;
		glFogi                      = 	dllFogi                      ;
		glFogiv                     = 	dllFogiv                     ;
		glFrontFace                 = 	dllFrontFace                 ;
		glFrustum                   = 	dllFrustum                   ;
		glGenLists                  = 	dllGenLists                  ;
		glGenTextures               = 	dllGenTextures               ;
		glGetBooleanv               = 	dllGetBooleanv               ;
		glGetClipPlane              = 	dllGetClipPlane              ;
		glGetDoublev                = 	dllGetDoublev                ;
		glGetError                  = 	dllGetError                  ;
		glGetFloatv                 = 	dllGetFloatv                 ;
		glGetIntegerv               = 	dllGetIntegerv               ;
		glGetLightfv                = 	dllGetLightfv                ;
		glGetLightiv                = 	dllGetLightiv                ;
		glGetMapdv                  = 	dllGetMapdv                  ;
		glGetMapfv                  = 	dllGetMapfv                  ;
		glGetMapiv                  = 	dllGetMapiv                  ;
		glGetMaterialfv             = 	dllGetMaterialfv             ;
		glGetMaterialiv             = 	dllGetMaterialiv             ;
		glGetPixelMapfv             = 	dllGetPixelMapfv             ;
		glGetPixelMapuiv            = 	dllGetPixelMapuiv            ;
		glGetPixelMapusv            = 	dllGetPixelMapusv            ;
		glGetPointerv               = 	dllGetPointerv               ;
		glGetPolygonStipple         = 	dllGetPolygonStipple         ;
		glGetString                 = 	dllGetString                 ;
		glGetTexEnvfv               = 	dllGetTexEnvfv               ;
		glGetTexEnviv               = 	dllGetTexEnviv               ;
		glGetTexGendv               = 	dllGetTexGendv               ;
		glGetTexGenfv               = 	dllGetTexGenfv               ;
		glGetTexGeniv               = 	dllGetTexGeniv               ;
		glGetTexImage               = 	dllGetTexImage               ;
		glGetTexLevelParameterfv    = 	dllGetTexLevelParameterfv    ;
		glGetTexLevelParameteriv    = 	dllGetTexLevelParameteriv    ;
		glGetTexParameterfv         = 	dllGetTexParameterfv         ;
		glGetTexParameteriv         = 	dllGetTexParameteriv         ;
		glHint                      = 	dllHint                      ;
		glIndexMask                 = 	dllIndexMask                 ;
		glIndexPointer              = 	dllIndexPointer              ;
		glIndexd                    = 	dllIndexd                    ;
		glIndexdv                   = 	dllIndexdv                   ;
		glIndexf                    = 	dllIndexf                    ;
		glIndexfv                   = 	dllIndexfv                   ;
		glIndexi                    = 	dllIndexi                    ;
		glIndexiv                   = 	dllIndexiv                   ;
		glIndexs                    = 	dllIndexs                    ;
		glIndexsv                   = 	dllIndexsv                   ;
		glIndexub                   = 	dllIndexub                   ;
		glIndexubv                  = 	dllIndexubv                  ;
		glInitNames                 = 	dllInitNames                 ;
		glInterleavedArrays         = 	dllInterleavedArrays         ;
		glIsEnabled                 = 	dllIsEnabled                 ;
		glIsList                    = 	dllIsList                    ;
		glIsTexture                 = 	dllIsTexture                 ;
		glLightModelf               = 	dllLightModelf               ;
		glLightModelfv              = 	dllLightModelfv              ;
		glLightModeli               = 	dllLightModeli               ;
		glLightModeliv              = 	dllLightModeliv              ;
		glLightf                    = 	dllLightf                    ;
		glLightfv                   = 	dllLightfv                   ;
		glLighti                    = 	dllLighti                    ;
		glLightiv                   = 	dllLightiv                   ;
		glLineStipple               = 	dllLineStipple               ;
		glLineWidth                 = 	dllLineWidth                 ;
		glListBase                  = 	dllListBase                  ;
		glLoadIdentity              = 	dllLoadIdentity              ;
		glLoadMatrixd               = 	dllLoadMatrixd               ;
		glLoadMatrixf               = 	dllLoadMatrixf               ;
		glLoadName                  = 	dllLoadName                  ;
		glLogicOp                   = 	dllLogicOp                   ;
		glMap1d                     = 	dllMap1d                     ;
		glMap1f                     = 	dllMap1f                     ;
		glMap2d                     = 	dllMap2d                     ;
		glMap2f                     = 	dllMap2f                     ;
		glMapGrid1d                 = 	dllMapGrid1d                 ;
		glMapGrid1f                 = 	dllMapGrid1f                 ;
		glMapGrid2d                 = 	dllMapGrid2d                 ;
		glMapGrid2f                 = 	dllMapGrid2f                 ;
		glMaterialf                 = 	dllMaterialf                 ;
		glMaterialfv                = 	dllMaterialfv                ;
		glMateriali                 = 	dllMateriali                 ;
		glMaterialiv                = 	dllMaterialiv                ;
		glMatrixMode                = 	dllMatrixMode                ;
		glMultMatrixd               = 	dllMultMatrixd               ;
		glMultMatrixf               = 	dllMultMatrixf               ;
		glNewList                   = 	dllNewList                   ;
		glNormal3b                  = 	dllNormal3b                  ;
		glNormal3bv                 = 	dllNormal3bv                 ;
		glNormal3d                  = 	dllNormal3d                  ;
		glNormal3dv                 = 	dllNormal3dv                 ;
		glNormal3f                  = 	dllNormal3f                  ;
		glNormal3fv                 = 	dllNormal3fv                 ;
		glNormal3i                  = 	dllNormal3i                  ;
		glNormal3iv                 = 	dllNormal3iv                 ;
		glNormal3s                  = 	dllNormal3s                  ;
		glNormal3sv                 = 	dllNormal3sv                 ;
		glNormalPointer             = 	dllNormalPointer             ;
		glOrtho                     = 	dllOrtho                     ;
		glPassThrough               = 	dllPassThrough               ;
		glPixelMapfv                = 	dllPixelMapfv                ;
		glPixelMapuiv               = 	dllPixelMapuiv               ;
		glPixelMapusv               = 	dllPixelMapusv               ;
		glPixelStoref               = 	dllPixelStoref               ;
		glPixelStorei               = 	dllPixelStorei               ;
		glPixelTransferf            = 	dllPixelTransferf            ;
		glPixelTransferi            = 	dllPixelTransferi            ;
		glPixelZoom                 = 	dllPixelZoom                 ;
		glPointSize                 = 	dllPointSize                 ;
		glPolygonMode               = 	dllPolygonMode               ;
		glPolygonOffset             = 	dllPolygonOffset             ;
		glPolygonStipple            = 	dllPolygonStipple            ;
		glPopAttrib                 = 	dllPopAttrib                 ;
		glPopClientAttrib           = 	dllPopClientAttrib           ;
		glPopMatrix                 = 	dllPopMatrix                 ;
		glPopName                   = 	dllPopName                   ;
		glPrioritizeTextures        = 	dllPrioritizeTextures        ;
		glPushAttrib                = 	dllPushAttrib                ;
		glPushClientAttrib          = 	dllPushClientAttrib          ;
		glPushMatrix                = 	dllPushMatrix                ;
		glPushName                  = 	dllPushName                  ;
		glRasterPos2d               = 	dllRasterPos2d               ;
		glRasterPos2dv              = 	dllRasterPos2dv              ;
		glRasterPos2f               = 	dllRasterPos2f               ;
		glRasterPos2fv              = 	dllRasterPos2fv              ;
		glRasterPos2i               = 	dllRasterPos2i               ;
		glRasterPos2iv              = 	dllRasterPos2iv              ;
		glRasterPos2s               = 	dllRasterPos2s               ;
		glRasterPos2sv              = 	dllRasterPos2sv              ;
		glRasterPos3d               = 	dllRasterPos3d               ;
		glRasterPos3dv              = 	dllRasterPos3dv              ;
		glRasterPos3f               = 	dllRasterPos3f               ;
		glRasterPos3fv              = 	dllRasterPos3fv              ;
		glRasterPos3i               = 	dllRasterPos3i               ;
		glRasterPos3iv              = 	dllRasterPos3iv              ;
		glRasterPos3s               = 	dllRasterPos3s               ;
		glRasterPos3sv              = 	dllRasterPos3sv              ;
		glRasterPos4d               = 	dllRasterPos4d               ;
		glRasterPos4dv              = 	dllRasterPos4dv              ;
		glRasterPos4f               = 	dllRasterPos4f               ;
		glRasterPos4fv              = 	dllRasterPos4fv              ;
		glRasterPos4i               = 	dllRasterPos4i               ;
		glRasterPos4iv              = 	dllRasterPos4iv              ;
		glRasterPos4s               = 	dllRasterPos4s               ;
		glRasterPos4sv              = 	dllRasterPos4sv              ;
		glReadBuffer                = 	dllReadBuffer                ;
		glReadPixels                = 	dllReadPixels                ;
		glRectd                     = 	dllRectd                     ;
		glRectdv                    = 	dllRectdv                    ;
		glRectf                     = 	dllRectf                     ;
		glRectfv                    = 	dllRectfv                    ;
		glRecti                     = 	dllRecti                     ;
		glRectiv                    = 	dllRectiv                    ;
		glRects                     = 	dllRects                     ;
		glRectsv                    = 	dllRectsv                    ;
		glRenderMode                = 	dllRenderMode                ;
		glRotated                   = 	dllRotated                   ;
		glRotatef                   = 	dllRotatef                   ;
		glScaled                    = 	dllScaled                    ;
		glScalef                    = 	dllScalef                    ;
		glScissor                   = 	dllScissor                   ;
		glSelectBuffer              = 	dllSelectBuffer              ;
		glShadeModel                = 	dllShadeModel                ;
		glStencilFunc               = 	dllStencilFunc               ;
		glStencilMask               = 	dllStencilMask               ;
		glStencilOp                 = 	dllStencilOp                 ;
		glTexCoord1d                = 	dllTexCoord1d                ;
		glTexCoord1dv               = 	dllTexCoord1dv               ;
		glTexCoord1f                = 	dllTexCoord1f                ;
		glTexCoord1fv               = 	dllTexCoord1fv               ;
		glTexCoord1i                = 	dllTexCoord1i                ;
		glTexCoord1iv               = 	dllTexCoord1iv               ;
		glTexCoord1s                = 	dllTexCoord1s                ;
		glTexCoord1sv               = 	dllTexCoord1sv               ;
		glTexCoord2d                = 	dllTexCoord2d                ;
		glTexCoord2dv               = 	dllTexCoord2dv               ;
		glTexCoord2f                = 	dllTexCoord2f                ;
		glTexCoord2fv               = 	dllTexCoord2fv               ;
		glTexCoord2i                = 	dllTexCoord2i                ;
		glTexCoord2iv               = 	dllTexCoord2iv               ;
		glTexCoord2s                = 	dllTexCoord2s                ;
		glTexCoord2sv               = 	dllTexCoord2sv               ;
		glTexCoord3d                = 	dllTexCoord3d                ;
		glTexCoord3dv               = 	dllTexCoord3dv               ;
		glTexCoord3f                = 	dllTexCoord3f                ;
		glTexCoord3fv               = 	dllTexCoord3fv               ;
		glTexCoord3i                = 	dllTexCoord3i                ;
		glTexCoord3iv               = 	dllTexCoord3iv               ;
		glTexCoord3s                = 	dllTexCoord3s                ;
		glTexCoord3sv               = 	dllTexCoord3sv               ;
		glTexCoord4d                = 	dllTexCoord4d                ;
		glTexCoord4dv               = 	dllTexCoord4dv               ;
		glTexCoord4f                = 	dllTexCoord4f                ;
		glTexCoord4fv               = 	dllTexCoord4fv               ;
		glTexCoord4i                = 	dllTexCoord4i                ;
		glTexCoord4iv               = 	dllTexCoord4iv               ;
		glTexCoord4s                = 	dllTexCoord4s                ;
		glTexCoord4sv               = 	dllTexCoord4sv               ;
		glTexCoordPointer           = 	dllTexCoordPointer           ;
		glTexEnvf                   = 	dllTexEnvf                   ;
		glTexEnvfv                  = 	dllTexEnvfv                  ;
		glTexEnvi                   = 	dllTexEnvi                   ;
		glTexEnviv                  = 	dllTexEnviv                  ;
		glTexGend                   = 	dllTexGend                   ;
		glTexGendv                  = 	dllTexGendv                  ;
		glTexGenf                   = 	dllTexGenf                   ;
		glTexGenfv                  = 	dllTexGenfv                  ;
		glTexGeni                   = 	dllTexGeni                   ;
		glTexGeniv                  = 	dllTexGeniv                  ;
		glTexImage1D                = 	dllTexImage1D                ;
		glTexImage2D                = 	dllTexImage2D                ;
		glTexParameterf             = 	dllTexParameterf             ;
		glTexParameterfv            = 	dllTexParameterfv            ;
		glTexParameteri             = 	dllTexParameteri             ;
		glTexParameteriv            = 	dllTexParameteriv            ;
		glTexSubImage1D             = 	dllTexSubImage1D             ;
		glTexSubImage2D             = 	dllTexSubImage2D             ;
		glTranslated                = 	dllTranslated                ;
		glTranslatef                = 	dllTranslatef                ;
		glVertex2d                  = 	dllVertex2d                  ;
		glVertex2dv                 = 	dllVertex2dv                 ;
		glVertex2f                  = 	dllVertex2f                  ;
		glVertex2fv                 = 	dllVertex2fv                 ;
		glVertex2i                  = 	dllVertex2i                  ;
		glVertex2iv                 = 	dllVertex2iv                 ;
		glVertex2s                  = 	dllVertex2s                  ;
		glVertex2sv                 = 	dllVertex2sv                 ;
		glVertex3d                  = 	dllVertex3d                  ;
		glVertex3dv                 = 	dllVertex3dv                 ;
		glVertex3f                  = 	dllVertex3f                  ;
		glVertex3fv                 = 	dllVertex3fv                 ;
		glVertex3i                  = 	dllVertex3i                  ;
		glVertex3iv                 = 	dllVertex3iv                 ;
		glVertex3s                  = 	dllVertex3s                  ;
		glVertex3sv                 = 	dllVertex3sv                 ;
		glVertex4d                  = 	dllVertex4d                  ;
		glVertex4dv                 = 	dllVertex4dv                 ;
		glVertex4f                  = 	dllVertex4f                  ;
		glVertex4fv                 = 	dllVertex4fv                 ;
		glVertex4i                  = 	dllVertex4i                  ;
		glVertex4iv                 = 	dllVertex4iv                 ;
		glVertex4s                  = 	dllVertex4s                  ;
		glVertex4sv                 = 	dllVertex4sv                 ;
		glVertexPointer             = 	dllVertexPointer             ;
		glViewport                  = 	dllViewport                  ;
	}
}


void GLimp_LogNewFrame( void )
{
	fprintf( glw_state.log_fp, "*** R_BeginFrame ***\n" );
}

#pragma warning (default : 4113 4133 4047 )



