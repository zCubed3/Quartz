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
** QGL.H
*/

#ifndef __QGL_H__
#define __QGL_H__

#ifdef _WIN32
#  include <windows.h>
#endif

#include <GL/gl.h>

#ifdef __linux__
//#include <GL/fxmesa.h>
#include <GL/glx.h>
#endif

qboolean QGL_Init( const char *dllname );
void     QGL_Shutdown( void );

#ifndef APIENTRY
#  define APIENTRY
#endif

extern  void ( APIENTRY * glAccum )(GLenum op, GLfloat value);
extern  void ( APIENTRY * glAlphaFunc )(GLenum func, GLclampf ref);
extern  GLboolean ( APIENTRY * glAreTexturesResident )(GLsizei n, const GLuint *textures, GLboolean *residences);
extern  void ( APIENTRY * glArrayElement )(GLint i);
extern  void ( APIENTRY * glBegin )(GLenum mode);
extern  void ( APIENTRY * glBindTexture )(GLenum target, GLuint texture);
extern  void ( APIENTRY * glBitmap )(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern  void ( APIENTRY * glBlendFunc )(GLenum sfactor, GLenum dfactor);
extern  void ( APIENTRY * glCallList )(GLuint list);
extern  void ( APIENTRY * glCallLists )(GLsizei n, GLenum type, const GLvoid *lists);
extern  void ( APIENTRY * glClear )(GLbitfield mask);
extern  void ( APIENTRY * glClearAccum )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern  void ( APIENTRY * glClearColor )(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern  void ( APIENTRY * glClearDepth )(GLclampd depth);
extern  void ( APIENTRY * glClearIndex )(GLfloat c);
extern  void ( APIENTRY * glClearStencil )(GLint s);
extern  void ( APIENTRY * glClipPlane )(GLenum plane, const GLdouble *equation);
extern  void ( APIENTRY * glColor3b )(GLbyte red, GLbyte green, GLbyte blue);
extern  void ( APIENTRY * glColor3bv )(const GLbyte *v);
extern  void ( APIENTRY * glColor3d )(GLdouble red, GLdouble green, GLdouble blue);
extern  void ( APIENTRY * glColor3dv )(const GLdouble *v);
extern  void ( APIENTRY * glColor3f )(GLfloat red, GLfloat green, GLfloat blue);
extern  void ( APIENTRY * glColor3fv )(const GLfloat *v);
extern  void ( APIENTRY * glColor3i )(GLint red, GLint green, GLint blue);
extern  void ( APIENTRY * glColor3iv )(const GLint *v);
extern  void ( APIENTRY * glColor3s )(GLshort red, GLshort green, GLshort blue);
extern  void ( APIENTRY * glColor3sv )(const GLshort *v);
extern  void ( APIENTRY * glColor3ub )(GLubyte red, GLubyte green, GLubyte blue);
extern  void ( APIENTRY * glColor3ubv )(const GLubyte *v);
extern  void ( APIENTRY * glColor3ui )(GLuint red, GLuint green, GLuint blue);
extern  void ( APIENTRY * glColor3uiv )(const GLuint *v);
extern  void ( APIENTRY * glColor3us )(GLushort red, GLushort green, GLushort blue);
extern  void ( APIENTRY * glColor3usv )(const GLushort *v);
extern  void ( APIENTRY * glColor4b )(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern  void ( APIENTRY * glColor4bv )(const GLbyte *v);
extern  void ( APIENTRY * glColor4d )(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern  void ( APIENTRY * glColor4dv )(const GLdouble *v);
extern  void ( APIENTRY * glColor4f )(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern  void ( APIENTRY * glColor4fv )(const GLfloat *v);
extern  void ( APIENTRY * glColor4i )(GLint red, GLint green, GLint blue, GLint alpha);
extern  void ( APIENTRY * glColor4iv )(const GLint *v);
extern  void ( APIENTRY * glColor4s )(GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern  void ( APIENTRY * glColor4sv )(const GLshort *v);
extern  void ( APIENTRY * glColor4ub )(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern  void ( APIENTRY * glColor4ubv )(const GLubyte *v);
extern  void ( APIENTRY * glColor4ui )(GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern  void ( APIENTRY * glColor4uiv )(const GLuint *v);
extern  void ( APIENTRY * glColor4us )(GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern  void ( APIENTRY * glColor4usv )(const GLushort *v);
extern  void ( APIENTRY * glColorMask )(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern  void ( APIENTRY * glColorMaterial )(GLenum face, GLenum mode);
extern  void ( APIENTRY * glColorPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glCopyPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern  void ( APIENTRY * glCopyTexImage1D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
extern  void ( APIENTRY * glCopyTexImage2D )(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern  void ( APIENTRY * glCopyTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern  void ( APIENTRY * glCopyTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern  void ( APIENTRY * glCullFace )(GLenum mode);
extern  void ( APIENTRY * glDeleteLists )(GLuint list, GLsizei range);
extern  void ( APIENTRY * glDeleteTextures )(GLsizei n, const GLuint *textures);
extern  void ( APIENTRY * glDepthFunc )(GLenum func);
extern  void ( APIENTRY * glDepthMask )(GLboolean flag);
extern  void ( APIENTRY * glDepthRange )(GLclampd zNear, GLclampd zFar);
extern  void ( APIENTRY * glDisable )(GLenum cap);
extern  void ( APIENTRY * glDisableClientState )(GLenum array);
extern  void ( APIENTRY * glDrawArrays )(GLenum mode, GLint first, GLsizei count);
extern  void ( APIENTRY * glDrawBuffer )(GLenum mode);
extern  void ( APIENTRY * glDrawElements )(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern  void ( APIENTRY * glDrawPixels )(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * glEdgeFlag )(GLboolean flag);
extern  void ( APIENTRY * glEdgeFlagPointer )(GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glEdgeFlagv )(const GLboolean *flag);
extern  void ( APIENTRY * glEnable )(GLenum cap);
extern  void ( APIENTRY * glEnableClientState )(GLenum array);
extern  void ( APIENTRY * glEnd )(void);
extern  void ( APIENTRY * glEndList )(void);
extern  void ( APIENTRY * glEvalCoord1d )(GLdouble u);
extern  void ( APIENTRY * glEvalCoord1dv )(const GLdouble *u);
extern  void ( APIENTRY * glEvalCoord1f )(GLfloat u);
extern  void ( APIENTRY * glEvalCoord1fv )(const GLfloat *u);
extern  void ( APIENTRY * glEvalCoord2d )(GLdouble u, GLdouble v);
extern  void ( APIENTRY * glEvalCoord2dv )(const GLdouble *u);
extern  void ( APIENTRY * glEvalCoord2f )(GLfloat u, GLfloat v);
extern  void ( APIENTRY * glEvalCoord2fv )(const GLfloat *u);
extern  void ( APIENTRY * glEvalMesh1 )(GLenum mode, GLint i1, GLint i2);
extern  void ( APIENTRY * glEvalMesh2 )(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern  void ( APIENTRY * glEvalPoint1 )(GLint i);
extern  void ( APIENTRY * glEvalPoint2 )(GLint i, GLint j);
extern  void ( APIENTRY * glFeedbackBuffer )(GLsizei size, GLenum type, GLfloat *buffer);
extern  void ( APIENTRY * glFinish )(void);
extern  void ( APIENTRY * glFlush )(void);
extern  void ( APIENTRY * glFogf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * glFogfv )(GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glFogi )(GLenum pname, GLint param);
extern  void ( APIENTRY * glFogiv )(GLenum pname, const GLint *params);
extern  void ( APIENTRY * glFrontFace )(GLenum mode);
extern  void ( APIENTRY * glFrustum )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern  GLuint ( APIENTRY * glGenLists )(GLsizei range);
extern  void ( APIENTRY * glGenTextures )(GLsizei n, GLuint *textures);
extern  void ( APIENTRY * glGetBooleanv )(GLenum pname, GLboolean *params);
extern  void ( APIENTRY * glGetClipPlane )(GLenum plane, GLdouble *equation);
extern  void ( APIENTRY * glGetDoublev )(GLenum pname, GLdouble *params);
extern  GLenum ( APIENTRY * glGetError )(void);
extern  void ( APIENTRY * glGetFloatv )(GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetIntegerv )(GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetLightfv )(GLenum light, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetLightiv )(GLenum light, GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetMapdv )(GLenum target, GLenum query, GLdouble *v);
extern  void ( APIENTRY * glGetMapfv )(GLenum target, GLenum query, GLfloat *v);
extern  void ( APIENTRY * glGetMapiv )(GLenum target, GLenum query, GLint *v);
extern  void ( APIENTRY * glGetMaterialfv )(GLenum face, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetMaterialiv )(GLenum face, GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetPixelMapfv )(GLenum map, GLfloat *values);
extern  void ( APIENTRY * glGetPixelMapuiv )(GLenum map, GLuint *values);
extern  void ( APIENTRY * glGetPixelMapusv )(GLenum map, GLushort *values);
extern  void ( APIENTRY * glGetPointerv )(GLenum pname, GLvoid* *params);
extern  void ( APIENTRY * glGetPolygonStipple )(GLubyte *mask);
extern  const GLubyte * ( APIENTRY * glGetString )(GLenum name);
extern  void ( APIENTRY * glGetTexEnvfv )(GLenum target, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetTexEnviv )(GLenum target, GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetTexGendv )(GLenum coord, GLenum pname, GLdouble *params);
extern  void ( APIENTRY * glGetTexGenfv )(GLenum coord, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetTexGeniv )(GLenum coord, GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetTexImage )(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern  void ( APIENTRY * glGetTexLevelParameterfv )(GLenum target, GLint level, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetTexLevelParameteriv )(GLenum target, GLint level, GLenum pname, GLint *params);
extern  void ( APIENTRY * glGetTexParameterfv )(GLenum target, GLenum pname, GLfloat *params);
extern  void ( APIENTRY * glGetTexParameteriv )(GLenum target, GLenum pname, GLint *params);
extern  void ( APIENTRY * glHint )(GLenum target, GLenum mode);
extern  void ( APIENTRY * glIndexMask )(GLuint mask);
extern  void ( APIENTRY * glIndexPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glIndexd )(GLdouble c);
extern  void ( APIENTRY * glIndexdv )(const GLdouble *c);
extern  void ( APIENTRY * glIndexf )(GLfloat c);
extern  void ( APIENTRY * glIndexfv )(const GLfloat *c);
extern  void ( APIENTRY * glIndexi )(GLint c);
extern  void ( APIENTRY * glIndexiv )(const GLint *c);
extern  void ( APIENTRY * glIndexs )(GLshort c);
extern  void ( APIENTRY * glIndexsv )(const GLshort *c);
extern  void ( APIENTRY * glIndexub )(GLubyte c);
extern  void ( APIENTRY * glIndexubv )(const GLubyte *c);
extern  void ( APIENTRY * glInitNames )(void);
extern  void ( APIENTRY * glInterleavedArrays )(GLenum format, GLsizei stride, const GLvoid *pointer);
extern  GLboolean ( APIENTRY * glIsEnabled )(GLenum cap);
extern  GLboolean ( APIENTRY * glIsList )(GLuint list);
extern  GLboolean ( APIENTRY * glIsTexture )(GLuint texture);
extern  void ( APIENTRY * glLightModelf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * glLightModelfv )(GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glLightModeli )(GLenum pname, GLint param);
extern  void ( APIENTRY * glLightModeliv )(GLenum pname, const GLint *params);
extern  void ( APIENTRY * glLightf )(GLenum light, GLenum pname, GLfloat param);
extern  void ( APIENTRY * glLightfv )(GLenum light, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glLighti )(GLenum light, GLenum pname, GLint param);
extern  void ( APIENTRY * glLightiv )(GLenum light, GLenum pname, const GLint *params);
extern  void ( APIENTRY * glLineStipple )(GLint factor, GLushort pattern);
extern  void ( APIENTRY * glLineWidth )(GLfloat width);
extern  void ( APIENTRY * glListBase )(GLuint base);
extern  void ( APIENTRY * glLoadIdentity )(void);
extern  void ( APIENTRY * glLoadMatrixd )(const GLdouble *m);
extern  void ( APIENTRY * glLoadMatrixf )(const GLfloat *m);
extern  void ( APIENTRY * glLoadName )(GLuint name);
extern  void ( APIENTRY * glLogicOp )(GLenum opcode);
extern  void ( APIENTRY * glMap1d )(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern  void ( APIENTRY * glMap1f )(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern  void ( APIENTRY * glMap2d )(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern  void ( APIENTRY * glMap2f )(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern  void ( APIENTRY * glMapGrid1d )(GLint un, GLdouble u1, GLdouble u2);
extern  void ( APIENTRY * glMapGrid1f )(GLint un, GLfloat u1, GLfloat u2);
extern  void ( APIENTRY * glMapGrid2d )(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern  void ( APIENTRY * glMapGrid2f )(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern  void ( APIENTRY * glMaterialf )(GLenum face, GLenum pname, GLfloat param);
extern  void ( APIENTRY * glMaterialfv )(GLenum face, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glMateriali )(GLenum face, GLenum pname, GLint param);
extern  void ( APIENTRY * glMaterialiv )(GLenum face, GLenum pname, const GLint *params);
extern  void ( APIENTRY * glMatrixMode )(GLenum mode);
extern  void ( APIENTRY * glMultMatrixd )(const GLdouble *m);
extern  void ( APIENTRY * glMultMatrixf )(const GLfloat *m);
extern  void ( APIENTRY * glNewList )(GLuint list, GLenum mode);
extern  void ( APIENTRY * glNormal3b )(GLbyte nx, GLbyte ny, GLbyte nz);
extern  void ( APIENTRY * glNormal3bv )(const GLbyte *v);
extern  void ( APIENTRY * glNormal3d )(GLdouble nx, GLdouble ny, GLdouble nz);
extern  void ( APIENTRY * glNormal3dv )(const GLdouble *v);
extern  void ( APIENTRY * glNormal3f )(GLfloat nx, GLfloat ny, GLfloat nz);
extern  void ( APIENTRY * glNormal3fv )(const GLfloat *v);
extern  void ( APIENTRY * glNormal3i )(GLint nx, GLint ny, GLint nz);
extern  void ( APIENTRY * glNormal3iv )(const GLint *v);
extern  void ( APIENTRY * glNormal3s )(GLshort nx, GLshort ny, GLshort nz);
extern  void ( APIENTRY * glNormal3sv )(const GLshort *v);
extern  void ( APIENTRY * glNormalPointer )(GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glOrtho )(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern  void ( APIENTRY * glPassThrough )(GLfloat token);
extern  void ( APIENTRY * glPixelMapfv )(GLenum map, GLsizei mapsize, const GLfloat *values);
extern  void ( APIENTRY * glPixelMapuiv )(GLenum map, GLsizei mapsize, const GLuint *values);
extern  void ( APIENTRY * glPixelMapusv )(GLenum map, GLsizei mapsize, const GLushort *values);
extern  void ( APIENTRY * glPixelStoref )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * glPixelStorei )(GLenum pname, GLint param);
extern  void ( APIENTRY * glPixelTransferf )(GLenum pname, GLfloat param);
extern  void ( APIENTRY * glPixelTransferi )(GLenum pname, GLint param);
extern  void ( APIENTRY * glPixelZoom )(GLfloat xfactor, GLfloat yfactor);
extern  void ( APIENTRY * glPointSize )(GLfloat size);
extern  void ( APIENTRY * glPolygonMode )(GLenum face, GLenum mode);
extern  void ( APIENTRY * glPolygonOffset )(GLfloat factor, GLfloat units);
extern  void ( APIENTRY * glPolygonStipple )(const GLubyte *mask);
extern  void ( APIENTRY * glPopAttrib )(void);
extern  void ( APIENTRY * glPopClientAttrib )(void);
extern  void ( APIENTRY * glPopMatrix )(void);
extern  void ( APIENTRY * glPopName )(void);
extern  void ( APIENTRY * glPrioritizeTextures )(GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern  void ( APIENTRY * glPushAttrib )(GLbitfield mask);
extern  void ( APIENTRY * glPushClientAttrib )(GLbitfield mask);
extern  void ( APIENTRY * glPushMatrix )(void);
extern  void ( APIENTRY * glPushName )(GLuint name);
extern  void ( APIENTRY * glRasterPos2d )(GLdouble x, GLdouble y);
extern  void ( APIENTRY * glRasterPos2dv )(const GLdouble *v);
extern  void ( APIENTRY * glRasterPos2f )(GLfloat x, GLfloat y);
extern  void ( APIENTRY * glRasterPos2fv )(const GLfloat *v);
extern  void ( APIENTRY * glRasterPos2i )(GLint x, GLint y);
extern  void ( APIENTRY * glRasterPos2iv )(const GLint *v);
extern  void ( APIENTRY * glRasterPos2s )(GLshort x, GLshort y);
extern  void ( APIENTRY * glRasterPos2sv )(const GLshort *v);
extern  void ( APIENTRY * glRasterPos3d )(GLdouble x, GLdouble y, GLdouble z);
extern  void ( APIENTRY * glRasterPos3dv )(const GLdouble *v);
extern  void ( APIENTRY * glRasterPos3f )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * glRasterPos3fv )(const GLfloat *v);
extern  void ( APIENTRY * glRasterPos3i )(GLint x, GLint y, GLint z);
extern  void ( APIENTRY * glRasterPos3iv )(const GLint *v);
extern  void ( APIENTRY * glRasterPos3s )(GLshort x, GLshort y, GLshort z);
extern  void ( APIENTRY * glRasterPos3sv )(const GLshort *v);
extern  void ( APIENTRY * glRasterPos4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern  void ( APIENTRY * glRasterPos4dv )(const GLdouble *v);
extern  void ( APIENTRY * glRasterPos4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern  void ( APIENTRY * glRasterPos4fv )(const GLfloat *v);
extern  void ( APIENTRY * glRasterPos4i )(GLint x, GLint y, GLint z, GLint w);
extern  void ( APIENTRY * glRasterPos4iv )(const GLint *v);
extern  void ( APIENTRY * glRasterPos4s )(GLshort x, GLshort y, GLshort z, GLshort w);
extern  void ( APIENTRY * glRasterPos4sv )(const GLshort *v);
extern  void ( APIENTRY * glReadBuffer )(GLenum mode);
extern  void ( APIENTRY * glReadPixels )(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern  void ( APIENTRY * glRectd )(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern  void ( APIENTRY * glRectdv )(const GLdouble *v1, const GLdouble *v2);
extern  void ( APIENTRY * glRectf )(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern  void ( APIENTRY * glRectfv )(const GLfloat *v1, const GLfloat *v2);
extern  void ( APIENTRY * glRecti )(GLint x1, GLint y1, GLint x2, GLint y2);
extern  void ( APIENTRY * glRectiv )(const GLint *v1, const GLint *v2);
extern  void ( APIENTRY * glRects )(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern  void ( APIENTRY * glRectsv )(const GLshort *v1, const GLshort *v2);
extern  GLint ( APIENTRY * glRenderMode )(GLenum mode);
extern  void ( APIENTRY * glRotated )(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern  void ( APIENTRY * glRotatef )(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * glScaled )(GLdouble x, GLdouble y, GLdouble z);
extern  void ( APIENTRY * glScalef )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * glScissor )(GLint x, GLint y, GLsizei width, GLsizei height);
extern  void ( APIENTRY * glSelectBuffer )(GLsizei size, GLuint *buffer);
extern  void ( APIENTRY * glShadeModel )(GLenum mode);
extern  void ( APIENTRY * glStencilFunc )(GLenum func, GLint ref, GLuint mask);
extern  void ( APIENTRY * glStencilMask )(GLuint mask);
extern  void ( APIENTRY * glStencilOp )(GLenum fail, GLenum zfail, GLenum zpass);
extern  void ( APIENTRY * glTexCoord1d )(GLdouble s);
extern  void ( APIENTRY * glTexCoord1dv )(const GLdouble *v);
extern  void ( APIENTRY * glTexCoord1f )(GLfloat s);
extern  void ( APIENTRY * glTexCoord1fv )(const GLfloat *v);
extern  void ( APIENTRY * glTexCoord1i )(GLint s);
extern  void ( APIENTRY * glTexCoord1iv )(const GLint *v);
extern  void ( APIENTRY * glTexCoord1s )(GLshort s);
extern  void ( APIENTRY * glTexCoord1sv )(const GLshort *v);
extern  void ( APIENTRY * glTexCoord2d )(GLdouble s, GLdouble t);
extern  void ( APIENTRY * glTexCoord2dv )(const GLdouble *v);
extern  void ( APIENTRY * glTexCoord2f )(GLfloat s, GLfloat t);
extern  void ( APIENTRY * glTexCoord2fv )(const GLfloat *v);
extern  void ( APIENTRY * glTexCoord2i )(GLint s, GLint t);
extern  void ( APIENTRY * glTexCoord2iv )(const GLint *v);
extern  void ( APIENTRY * glTexCoord2s )(GLshort s, GLshort t);
extern  void ( APIENTRY * glTexCoord2sv )(const GLshort *v);
extern  void ( APIENTRY * glTexCoord3d )(GLdouble s, GLdouble t, GLdouble r);
extern  void ( APIENTRY * glTexCoord3dv )(const GLdouble *v);
extern  void ( APIENTRY * glTexCoord3f )(GLfloat s, GLfloat t, GLfloat r);
extern  void ( APIENTRY * glTexCoord3fv )(const GLfloat *v);
extern  void ( APIENTRY * glTexCoord3i )(GLint s, GLint t, GLint r);
extern  void ( APIENTRY * glTexCoord3iv )(const GLint *v);
extern  void ( APIENTRY * glTexCoord3s )(GLshort s, GLshort t, GLshort r);
extern  void ( APIENTRY * glTexCoord3sv )(const GLshort *v);
extern  void ( APIENTRY * glTexCoord4d )(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern  void ( APIENTRY * glTexCoord4dv )(const GLdouble *v);
extern  void ( APIENTRY * glTexCoord4f )(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern  void ( APIENTRY * glTexCoord4fv )(const GLfloat *v);
extern  void ( APIENTRY * glTexCoord4i )(GLint s, GLint t, GLint r, GLint q);
extern  void ( APIENTRY * glTexCoord4iv )(const GLint *v);
extern  void ( APIENTRY * glTexCoord4s )(GLshort s, GLshort t, GLshort r, GLshort q);
extern  void ( APIENTRY * glTexCoord4sv )(const GLshort *v);
extern  void ( APIENTRY * glTexCoordPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glTexEnvf )(GLenum target, GLenum pname, GLfloat param);
extern  void ( APIENTRY * glTexEnvfv )(GLenum target, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glTexEnvi )(GLenum target, GLenum pname, GLint param);
extern  void ( APIENTRY * glTexEnviv )(GLenum target, GLenum pname, const GLint *params);
extern  void ( APIENTRY * glTexGend )(GLenum coord, GLenum pname, GLdouble param);
extern  void ( APIENTRY * glTexGendv )(GLenum coord, GLenum pname, const GLdouble *params);
extern  void ( APIENTRY * glTexGenf )(GLenum coord, GLenum pname, GLfloat param);
extern  void ( APIENTRY * glTexGenfv )(GLenum coord, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glTexGeni )(GLenum coord, GLenum pname, GLint param);
extern  void ( APIENTRY * glTexGeniv )(GLenum coord, GLenum pname, const GLint *params);
extern  void ( APIENTRY * glTexImage1D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * glTexImage2D )(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * glTexParameterf )(GLenum target, GLenum pname, GLfloat param);
extern  void ( APIENTRY * glTexParameterfv )(GLenum target, GLenum pname, const GLfloat *params);
extern  void ( APIENTRY * glTexParameteri )(GLenum target, GLenum pname, GLint param);
extern  void ( APIENTRY * glTexParameteriv )(GLenum target, GLenum pname, const GLint *params);
extern  void ( APIENTRY * glTexSubImage1D )(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * glTexSubImage2D )(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern  void ( APIENTRY * glTranslated )(GLdouble x, GLdouble y, GLdouble z);
extern  void ( APIENTRY * glTranslatef )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * glVertex2d )(GLdouble x, GLdouble y);
extern  void ( APIENTRY * glVertex2dv )(const GLdouble *v);
extern  void ( APIENTRY * glVertex2f )(GLfloat x, GLfloat y);
extern  void ( APIENTRY * glVertex2fv )(const GLfloat *v);
extern  void ( APIENTRY * glVertex2i )(GLint x, GLint y);
extern  void ( APIENTRY * glVertex2iv )(const GLint *v);
extern  void ( APIENTRY * glVertex2s )(GLshort x, GLshort y);
extern  void ( APIENTRY * glVertex2sv )(const GLshort *v);
extern  void ( APIENTRY * glVertex3d )(GLdouble x, GLdouble y, GLdouble z);
extern  void ( APIENTRY * glVertex3dv )(const GLdouble *v);
extern  void ( APIENTRY * glVertex3f )(GLfloat x, GLfloat y, GLfloat z);
extern  void ( APIENTRY * glVertex3fv )(const GLfloat *v);
extern  void ( APIENTRY * glVertex3i )(GLint x, GLint y, GLint z);
extern  void ( APIENTRY * glVertex3iv )(const GLint *v);
extern  void ( APIENTRY * glVertex3s )(GLshort x, GLshort y, GLshort z);
extern  void ( APIENTRY * glVertex3sv )(const GLshort *v);
extern  void ( APIENTRY * glVertex4d )(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern  void ( APIENTRY * glVertex4dv )(const GLdouble *v);
extern  void ( APIENTRY * glVertex4f )(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern  void ( APIENTRY * glVertex4fv )(const GLfloat *v);
extern  void ( APIENTRY * glVertex4i )(GLint x, GLint y, GLint z, GLint w);
extern  void ( APIENTRY * glVertex4iv )(const GLint *v);
extern  void ( APIENTRY * glVertex4s )(GLshort x, GLshort y, GLshort z, GLshort w);
extern  void ( APIENTRY * glVertex4sv )(const GLshort *v);
extern  void ( APIENTRY * glVertexPointer )(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern  void ( APIENTRY * glViewport )(GLint x, GLint y, GLsizei width, GLsizei height);

extern	void ( APIENTRY * glPointParameterfEXT)( GLenum param, GLfloat value );
extern	void ( APIENTRY * glPointParameterfvEXT)( GLenum param, const GLfloat *value );
extern	void ( APIENTRY * glColorTableEXT)( int, int, int, int, int, const void * );

extern	void ( APIENTRY * glLockArraysEXT) (int , int);
extern	void ( APIENTRY * glUnlockArraysEXT) (void);

extern	void ( APIENTRY * glMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );
extern	void ( APIENTRY * glSelectTextureSGIS)( GLenum );

extern	void ( APIENTRY * glActiveTextureARB)( GLenum );
extern	void ( APIENTRY * glClientActiveTextureARB)( GLenum );

/*
** extension constants
*/
#define GL_POINT_SIZE_MIN_EXT				0x8126
#define GL_POINT_SIZE_MAX_EXT				0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT	0x8128
#define GL_DISTANCE_ATTENUATION_EXT			0x8129

#ifdef __sgi
#define GL_SHARED_TEXTURE_PALETTE_EXT		GL_TEXTURE_COLOR_TABLE_SGI
#else
#define GL_SHARED_TEXTURE_PALETTE_EXT		0x81FB
#endif

#define GL_TEXTURE0_SGIS					0x835E
#define GL_TEXTURE1_SGIS					0x835F
#define GL_TEXTURE0_ARB						0x84C0
#define GL_TEXTURE1_ARB						0x84C1

extern int GL_TEXTURE0, GL_TEXTURE1;

#endif
