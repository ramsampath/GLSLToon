	/* -*- Mode: c++; c-indentation-style: stroustrup; c-basic-offset: 4 -*- */

/*
  vgl.c - Virtual OpenGL - redirect OpenGL stuff (extension support)

  Copyright (C) 2006 Patric Ljung
 
  This file is part of XGFX, a free eXtensible Graphics system.

  XGFX is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
 
  XGFX is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  The authors of this software:

    Patric Ljung, plg@itn.liu.se, plg@acm.org, plg@ieee.org

*/

#ifdef WIN32
#include <windows.h>
#endif

#include "vgl.h"

#include <stdio.h>

#if 0

typedef void (*__GLXextFuncPtr)(void);
__GLXextFuncPtr glXGetProcAddressARB (const char *);

typedef __GLXextFuncPtr __extfuncptr;
#define getProcAddress(n) glXGetProcAddressARB(n)

#else

#include <windows.h>

typedef int (*__wglExtFuncPtr)(void);
// __wglExtFuncPtr wglGetProcAddress (const char *);

typedef __wglExtFuncPtr __extfuncptr;
#define getProcAddress(n) wglGetProcAddress(n)

#endif


/*
  Internal prototypes
*/

static long __vgl_nomap_func();
static long __vgl_missing_func();

#define DECLARE_PFNGL_VGLPTR(t,n)	t n = (t)__vgl_nomap_func

DECLARE_PFNGL_VGLPTR(PFNGLBLENDEQUATIONSEPARATEPROC,	__vglBlendEquationSeparate);
DECLARE_PFNGL_VGLPTR(PFNGLDRAWBUFFERSPROC,		__vglDrawBuffers);
DECLARE_PFNGL_VGLPTR(PFNGLSTENCILOPSEPARATEPROC,	__vglStencilOpSeparate);
DECLARE_PFNGL_VGLPTR(PFNGLSTENCILFUNCSEPARATEPROC,	__vglStencilFuncSeparate);
DECLARE_PFNGL_VGLPTR(PFNGLSTENCILMASKSEPARATEPROC,	__vglStencilMaskSeparate);
DECLARE_PFNGL_VGLPTR(PFNGLATTACHSHADERPROC,		__vglAttachShader);
DECLARE_PFNGL_VGLPTR(PFNGLBINDATTRIBLOCATIONPROC,	__vglBindAttribLocation);
DECLARE_PFNGL_VGLPTR(PFNGLCOMPILESHADERPROC,		__vglCompileShader);
DECLARE_PFNGL_VGLPTR(PFNGLCREATEPROGRAMPROC,		__vglCreateProgram);
DECLARE_PFNGL_VGLPTR(PFNGLCREATESHADERPROC,		__vglCreateShader);
DECLARE_PFNGL_VGLPTR(PFNGLDELETEPROGRAMPROC,		__vglDeleteProgram);
DECLARE_PFNGL_VGLPTR(PFNGLDELETESHADERPROC,		__vglDeleteShader);
DECLARE_PFNGL_VGLPTR(PFNGLDETACHSHADERPROC,		__vglDetachShader);
DECLARE_PFNGL_VGLPTR(PFNGLDISABLEVERTEXATTRIBARRAYPROC,	__vglDisableVertexAttribArray);
DECLARE_PFNGL_VGLPTR(PFNGLENABLEVERTEXATTRIBARRAYPROC,	__vglEnableVertexAttribArray);
DECLARE_PFNGL_VGLPTR(PFNGLGETACTIVEATTRIBPROC,		__vglGetActiveAttrib);
DECLARE_PFNGL_VGLPTR(PFNGLGETACTIVEUNIFORMPROC,		__vglGetActiveUniform);
DECLARE_PFNGL_VGLPTR(PFNGLGETATTACHEDSHADERSPROC,	__vglGetAttachedShaders);
DECLARE_PFNGL_VGLPTR(PFNGLGETATTRIBLOCATIONPROC,	__vglGetAttribLocation);
DECLARE_PFNGL_VGLPTR(PFNGLGETPROGRAMIVPROC,		__vglGetProgramiv);
DECLARE_PFNGL_VGLPTR(PFNGLGETPROGRAMINFOLOGPROC,	__vglGetProgramInfoLog);
DECLARE_PFNGL_VGLPTR(PFNGLGETSHADERIVPROC,		__vglGetShaderiv);
DECLARE_PFNGL_VGLPTR(PFNGLGETSHADERINFOLOGPROC,		__vglGetShaderInfoLog);
DECLARE_PFNGL_VGLPTR(PFNGLGETSHADERSOURCEPROC,		__vglGetShaderSource);
DECLARE_PFNGL_VGLPTR(PFNGLGETUNIFORMLOCATIONPROC,	__vglGetUniformLocation);
DECLARE_PFNGL_VGLPTR(PFNGLGETUNIFORMFVPROC,		__vglGetUniformfv);
DECLARE_PFNGL_VGLPTR(PFNGLGETUNIFORMIVPROC,		__vglGetUniformiv);
DECLARE_PFNGL_VGLPTR(PFNGLGETVERTEXATTRIBDVPROC,	__vglGetVertexAttribdv);
DECLARE_PFNGL_VGLPTR(PFNGLGETVERTEXATTRIBFVPROC,	__vglGetVertexAttribfv);
DECLARE_PFNGL_VGLPTR(PFNGLGETVERTEXATTRIBIVPROC,	__vglGetVertexAttribiv);
DECLARE_PFNGL_VGLPTR(PFNGLGETVERTEXATTRIBPOINTERVPROC,	__vglGetVertexAttribPointerv);
DECLARE_PFNGL_VGLPTR(PFNGLISPROGRAMPROC,		__vglIsProgram);
DECLARE_PFNGL_VGLPTR(PFNGLISSHADERPROC,			__vglIsShader);
DECLARE_PFNGL_VGLPTR(PFNGLLINKPROGRAMPROC,		__vglLinkProgram);
DECLARE_PFNGL_VGLPTR(PFNGLSHADERSOURCEPROC,		__vglShaderSource);
DECLARE_PFNGL_VGLPTR(PFNGLUSEPROGRAMPROC,		__vglUseProgram);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM1FPROC,		__vglUniform1f);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM2FPROC,		__vglUniform2f);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM3FPROC,		__vglUniform3f);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM4FPROC,		__vglUniform4f);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM1IPROC,		__vglUniform1i);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM2IPROC,		__vglUniform2i);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM3IPROC,		__vglUniform3i);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM4IPROC,		__vglUniform4i);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM1FVPROC,		__vglUniform1fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM2FVPROC,		__vglUniform2fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM3FVPROC,		__vglUniform3fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM4FVPROC,		__vglUniform4fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM1IVPROC,		__vglUniform1iv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM2IVPROC,		__vglUniform2iv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM3IVPROC,		__vglUniform3iv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORM4IVPROC,		__vglUniform4iv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORMMATRIX2FVPROC,		__vglUniformMatrix2fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORMMATRIX3FVPROC,		__vglUniformMatrix3fv);
DECLARE_PFNGL_VGLPTR(PFNGLUNIFORMMATRIX4FVPROC,		__vglUniformMatrix4fv);
DECLARE_PFNGL_VGLPTR(PFNGLVALIDATEPROGRAMPROC,		__vglValidateProgram);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1DPROC,		__vglVertexAttrib1d);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1DVPROC,		__vglVertexAttrib1dv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1FPROC,		__vglVertexAttrib1f);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1FVPROC,		__vglVertexAttrib1fv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1SPROC,		__vglVertexAttrib1s);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB1SVPROC,		__vglVertexAttrib1sv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2DPROC,		__vglVertexAttrib2d);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2DVPROC,		__vglVertexAttrib2dv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2FPROC,		__vglVertexAttrib2f);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2FVPROC,		__vglVertexAttrib2fv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2SPROC,		__vglVertexAttrib2s);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB2SVPROC,		__vglVertexAttrib2sv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3DPROC,		__vglVertexAttrib3d);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3DVPROC,		__vglVertexAttrib3dv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3FPROC,		__vglVertexAttrib3f);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3FVPROC,		__vglVertexAttrib3fv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3SPROC,		__vglVertexAttrib3s);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB3SVPROC,		__vglVertexAttrib3sv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NBVPROC,		__vglVertexAttrib4Nbv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NIVPROC,		__vglVertexAttrib4Niv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NSVPROC,		__vglVertexAttrib4Nsv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NUBPROC,		__vglVertexAttrib4Nub);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NUBVPROC,	__vglVertexAttrib4Nubv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NUIVPROC,	__vglVertexAttrib4Nuiv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4NUSVPROC,	__vglVertexAttrib4Nusv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4BVPROC,		__vglVertexAttrib4bv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4DPROC,		__vglVertexAttrib4d);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4DVPROC,		__vglVertexAttrib4dv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4FPROC,		__vglVertexAttrib4f);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4FVPROC,		__vglVertexAttrib4fv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4IVPROC,		__vglVertexAttrib4iv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4SPROC,		__vglVertexAttrib4s);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4SVPROC,		__vglVertexAttrib4sv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4UBVPROC,		__vglVertexAttrib4ubv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4UIVPROC,		__vglVertexAttrib4uiv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIB4USVPROC,		__vglVertexAttrib4usv);
DECLARE_PFNGL_VGLPTR(PFNGLVERTEXATTRIBPOINTERPROC,	__vglVertexAttribPointer);

DECLARE_PFNGL_VGLPTR(PFNGLDELETEOBJECTARBPROC,		__vglDeleteObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLGETHANDLEARBPROC,		__vglGetHandleARB);
DECLARE_PFNGL_VGLPTR(PFNGLDETACHOBJECTARBPROC,		__vglDetachObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLCREATESHADEROBJECTARBPROC,	__vglCreateShaderObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLCREATEPROGRAMOBJECTARBPROC,	__glCreateProgramObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLATTACHOBJECTARBPROC,		__vglAttachObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLUSEPROGRAMOBJECTARBPROC,	__vglUseProgramObjectARB);
DECLARE_PFNGL_VGLPTR(PFNGLGETOBJECTPARAMETERFVARBPROC,	__vglGetObjectParameterfvARB);
DECLARE_PFNGL_VGLPTR(PFNGLGETOBJECTPARAMETERIVARBPROC,	__vglGetObjectParameterivARB);
DECLARE_PFNGL_VGLPTR(PFNGLGETINFOLOGARBPROC,		__vglGetInfoLogARB);
DECLARE_PFNGL_VGLPTR(PFNGLGETATTACHEDOBJECTSARBPROC,	__vglGetAttachedObjectsARB);


DECLARE_PFNGL_VGLPTR(PFNGLISRENDERBUFFEREXTPROC,	__vglIsRenderbufferEXT);
DECLARE_PFNGL_VGLPTR(PFNGLBINDRENDERBUFFEREXTPROC,	__vglBindRenderbufferEXT);
DECLARE_PFNGL_VGLPTR(PFNGLDELETERENDERBUFFERSEXTPROC,	__vglDeleteRenderbuffersEXT);
DECLARE_PFNGL_VGLPTR(PFNGLGENRENDERBUFFERSEXTPROC,	__vglGenRenderbuffersEXT);
DECLARE_PFNGL_VGLPTR(PFNGLRENDERBUFFERSTORAGEEXTPROC,	__vglRenderbufferStorageEXT);
DECLARE_PFNGL_VGLPTR(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC,	__vglGetRenderbufferParameterivEXT);
DECLARE_PFNGL_VGLPTR(PFNGLISFRAMEBUFFEREXTPROC,	__vglIsFramebufferEXT);
DECLARE_PFNGL_VGLPTR(PFNGLBINDFRAMEBUFFEREXTPROC,	__vglBindFramebufferEXT);
DECLARE_PFNGL_VGLPTR(PFNGLDELETEFRAMEBUFFERSEXTPROC,	__vglDeleteFramebuffersEXT);
DECLARE_PFNGL_VGLPTR(PFNGLGENFRAMEBUFFERSEXTPROC,	__vglGenFramebuffersEXT);
DECLARE_PFNGL_VGLPTR(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC,	__vglCheckFramebufferStatusEXT);
DECLARE_PFNGL_VGLPTR(PFNGLFRAMEBUFFERTEXTURE1DEXTPROC,	__vglFramebufferTexture1DEXT);
DECLARE_PFNGL_VGLPTR(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC,	__vglFramebufferTexture2DEXT);
DECLARE_PFNGL_VGLPTR(PFNGLFRAMEBUFFERTEXTURE3DEXTPROC,	__vglFramebufferTexture3DEXT);
DECLARE_PFNGL_VGLPTR(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC,	__vglFramebufferRenderbufferEXT);
DECLARE_PFNGL_VGLPTR(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC,	__vglGetFramebufferAttachmentParameterivEXT);
DECLARE_PFNGL_VGLPTR(PFNGLGENERATEMIPMAPEXTPROC,	__vglGenerateMipmapEXT);



typedef struct vgl_map_table_s {
    const char * fname;
    void * fptr;
} vgl_map_table_t;


static vgl_map_table_t __vgl_map_table[] = {
    { "glBlendEquationSeparate",	&__vglBlendEquationSeparate },
    { "glDrawBuffers",			&__vglDrawBuffers },
    { "glStencilOpSeparate",		&__vglStencilOpSeparate },
    { "glStencilFuncSeparate",		&__vglStencilFuncSeparate },
    { "glStencilMaskSeparate",		&__vglStencilMaskSeparate },
    { "glAttachShader",			&__vglAttachShader },
    { "glBindAttribLocation",		&__vglBindAttribLocation },
    { "glCompileShader",		&__vglCompileShader },
    { "glCreateProgram",		&__vglCreateProgram },
    { "glCreateShader",			&__vglCreateShader },
    { "glDeleteProgram",		&__vglDeleteProgram },
    { "glDeleteShader",			&__vglDeleteShader },
    { "glDetachShader",			&__vglDetachShader },
    { "glDisableVertexAttribArray",	&__vglDisableVertexAttribArray },
    { "glEnableVertexAttribArray",	&__vglEnableVertexAttribArray },
    { "glGetActiveAttrib",		&__vglGetActiveAttrib },
    { "glGetActiveUniform",		&__vglGetActiveUniform },
    { "glGetAttachedShaders",		&__vglGetAttachedShaders },
    { "glGetAttribLocation",		&__vglGetAttribLocation },
    { "glGetProgramiv",			&__vglGetProgramiv },
    { "glGetProgramInfoLog",		&__vglGetProgramInfoLog },
    { "glGetShaderiv",			&__vglGetShaderiv },
    { "glGetShaderInfoLog",		&__vglGetShaderInfoLog },
    { "glGetShaderSource",		&__vglGetShaderSource },
    { "glGetUniformLocation",		&__vglGetUniformLocation },
    { "glGetUniformfv",			&__vglGetUniformfv },
    { "glGetUniformiv",			&__vglGetUniformiv },
    { "glGetVertexAttribdv",		&__vglGetVertexAttribdv },
    { "glGetVertexAttribfv",		&__vglGetVertexAttribfv },
    { "glGetVertexAttribiv",		&__vglGetVertexAttribiv },
    { "glGetVertexAttribPointerv",	&__vglGetVertexAttribPointerv },
    { "glIsProgram",			&__vglIsProgram },
    { "glIsShader",			&__vglIsShader },
    { "glLinkProgram",			&__vglLinkProgram },
    { "glShaderSource",			&__vglShaderSource },
    { "glUseProgram",			&__vglUseProgram },
    { "glUniform1f",			&__vglUniform1f },
    { "glUniform2f",			&__vglUniform2f },
    { "glUniform3f",			&__vglUniform3f },
    { "glUniform4f",			&__vglUniform4f },
    { "glUniform1i",			&__vglUniform1i },
    { "glUniform2i",			&__vglUniform2i },
    { "glUniform3i",			&__vglUniform3i },
    { "glUniform4i",			&__vglUniform4i },
    { "glUniform1fv",			&__vglUniform1fv },
    { "glUniform2fv",			&__vglUniform2fv },
    { "glUniform3fv",			&__vglUniform3fv },
    { "glUniform4fv",			&__vglUniform4fv },
    { "glUniform1iv",			&__vglUniform1iv },
    { "glUniform2iv",			&__vglUniform2iv },
    { "glUniform3iv",			&__vglUniform3iv },
    { "glUniform4iv",			&__vglUniform4iv },
    { "glUniformMatrix2fv",		&__vglUniformMatrix2fv },
    { "glUniformMatrix3fv",		&__vglUniformMatrix3fv },
    { "glUniformMatrix4fv",		&__vglUniformMatrix4fv },
    { "glValidateProgram",		&__vglValidateProgram },
    { "glVertexAttrib1d",		&__vglVertexAttrib1d },
    { "glVertexAttrib1dv",		&__vglVertexAttrib1dv },
    { "glVertexAttrib1f",		&__vglVertexAttrib1f },
    { "glVertexAttrib1fv",		&__vglVertexAttrib1fv },
    { "glVertexAttrib1s",		&__vglVertexAttrib1s },
    { "glVertexAttrib1sv",		&__vglVertexAttrib1sv },
    { "glVertexAttrib2d",		&__vglVertexAttrib2d },
    { "glVertexAttrib2dv",		&__vglVertexAttrib2dv },
    { "glVertexAttrib2f",		&__vglVertexAttrib2f },
    { "glVertexAttrib2fv",		&__vglVertexAttrib2fv },
    { "glVertexAttrib2s",		&__vglVertexAttrib2s },
    { "glVertexAttrib2sv",		&__vglVertexAttrib2sv },
    { "glVertexAttrib3d",		&__vglVertexAttrib3d },
    { "glVertexAttrib3dv",		&__vglVertexAttrib3dv },
    { "glVertexAttrib3f",		&__vglVertexAttrib3f },
    { "glVertexAttrib3fv",		&__vglVertexAttrib3fv },
    { "glVertexAttrib3s",		&__vglVertexAttrib3s },
    { "glVertexAttrib3sv",		&__vglVertexAttrib3sv },
    { "glVertexAttrib4Nbv",		&__vglVertexAttrib4Nbv },
    { "glVertexAttrib4Niv",		&__vglVertexAttrib4Niv },
    { "glVertexAttrib4Nsv",		&__vglVertexAttrib4Nsv },
    { "glVertexAttrib4Nub",		&__vglVertexAttrib4Nub },
    { "glVertexAttrib4Nubv",		&__vglVertexAttrib4Nubv },
    { "glVertexAttrib4Nuiv",		&__vglVertexAttrib4Nuiv },
    { "glVertexAttrib4Nusv",		&__vglVertexAttrib4Nusv },
    { "glVertexAttrib4bv",		&__vglVertexAttrib4bv },
    { "glVertexAttrib4d",		&__vglVertexAttrib4d },
    { "glVertexAttrib4dv",		&__vglVertexAttrib4dv },
    { "glVertexAttrib4f",		&__vglVertexAttrib4f },
    { "glVertexAttrib4fv",		&__vglVertexAttrib4fv },
    { "glVertexAttrib4iv",		&__vglVertexAttrib4iv },
    { "glVertexAttrib4s",		&__vglVertexAttrib4s },
    { "glVertexAttrib4sv",		&__vglVertexAttrib4sv },
    { "glVertexAttrib4ubv",		&__vglVertexAttrib4ubv },
    { "glVertexAttrib4uiv",		&__vglVertexAttrib4uiv },
    { "glVertexAttrib4usv",		&__vglVertexAttrib4usv },
    { "glVertexAttribPointer",		&__vglVertexAttribPointer },

    { "glDeleteObjectARB",		&__vglDeleteObjectARB },
    { "glGetHandleARB",			&__vglGetHandleARB },
    { "glDetachObjectARB",		&__vglDetachObjectARB },
    { "glCreateShaderObjectARB",	&__vglCreateShaderObjectARB },
    { "glCreateProgramObjectARB",	&__glCreateProgramObjectARB },
    { "glAttachObjectARB",		&__vglAttachObjectARB },
    { "glUseProgramObjectARB",		&__vglUseProgramObjectARB },
    { "glGetObjectParameterfvARB",	&__vglGetObjectParameterfvARB },
    { "glGetObjectParameterivARB",	&__vglGetObjectParameterivARB },
    { "glGetInfoLogARB",		&__vglGetInfoLogARB },
    { "glGetAttachedObjectsARB",	&__vglGetAttachedObjectsARB },

    { "glIsRenderbufferEXT",		&__vglIsRenderbufferEXT },
    { "glBindRenderbufferEXT",		&__vglBindRenderbufferEXT },
    { "glDeleteRenderbuffersEXT",	&__vglDeleteRenderbuffersEXT },
    { "glGenRenderbuffersEXT",		&__vglGenRenderbuffersEXT },
    { "glRenderbufferStorageEXT",	&__vglRenderbufferStorageEXT },
    { "glGetRenderbufferParameterivEXT",&__vglGetRenderbufferParameterivEXT },
    { "glIsFramebufferEXT",		&__vglIsFramebufferEXT },
    { "glBindFramebufferEXT",		&__vglBindFramebufferEXT },
    { "glDeleteFramebuffersEXT",	&__vglDeleteFramebuffersEXT },
    { "glGenFramebuffersEXT",		&__vglGenFramebuffersEXT },
    { "glCheckFramebufferStatusEXT",	&__vglCheckFramebufferStatusEXT },
    { "glFramebufferTexture1DEXT",	&__vglFramebufferTexture1DEXT },
    { "glFramebufferTexture2DEXT",	&__vglFramebufferTexture2DEXT },
    { "glFramebufferTexture3DEXT",	&__vglFramebufferTexture3DEXT },
    { "glFramebufferRenderbufferEXT",	&__vglFramebufferRenderbufferEXT },
    { "glGetFramebufferAttachmentParameterivEXT",	&__vglGetFramebufferAttachmentParameterivEXT },
    { "glGenerateMipmapEXT",		&__vglGenerateMipmapEXT },

    { 0, 0 }
};


int vglInit(FILE * out)
{
    vgl_map_table_t * tab = __vgl_map_table;
    int missing = 0;

    while (tab->fname) {
	__extfuncptr * fptr = (__extfuncptr *)tab->fptr;

	*fptr = (__extfuncptr)getProcAddress(tab->fname);

	if (*fptr == 0) {
	    *fptr = (__extfuncptr)__vgl_missing_func;

	    if (out) {
		if (!missing) {
		    fprintf(out, "vglInit: Missing extension functions:\n", tab->fname);
		}
		fprintf(out, "  %s\n", tab->fname);
	    }

	    missing++;
	}

	tab++;
    }

    if (out && missing) {
	fprintf(out, "%d functions are missing\n", missing);
    }

    return -missing;
}

long __vgl_nomap_func()
{
    fprintf(stderr, "vgl_nomap_func: OpenGL extensions not initialized\n");

    return 0;
}

long __vgl_missing_func()
{
    fprintf(stderr, "vgl_missing_func: OpenGL extension is missing\n");

    return 0;
}
