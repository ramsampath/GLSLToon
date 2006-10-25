/* -*- Mode: c++; c-indentation-style: stroustrup; c-basic-offset: 4 -*- */

/*
  vgl.h - Virtual OpenGL - redirect OpenGL stuff (extension support)

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

#ifndef _vgl_h_
#define _vgl_h_

#include <stdio.h>

#include <GL/gl.h>
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
#include "glext.h"

#ifndef VGL_OPENGL2_0_EXTENSIONS
#define VGL_OPENGL2_0_EXTENSIONS

#define glBlendEquationSeparate		(*__vglBlendEquationSeparate)
#define glDrawBuffers			(*__vglDrawBuffers)
#define glStencilOpSeparate		(*__vglStencilOpSeparate)
#define glStencilFuncSeparate		(*__vglStencilFuncSeparate)
#define glStencilMaskSeparate		(*__vglStencilMaskSeparate)
#define glAttachShader			(*__vglAttachShader)
#define glBindAttribLocation		(*__vglBindAttribLocation)
#define glCompileShader			(*__vglCompileShader)
#define glCreateProgram			(*__vglCreateProgram)
#define glCreateShader			(*__vglCreateShader)
#define glDeleteProgram			(*__vglDeleteProgram)
#define glDeleteShader			(*__vglDeleteShader)
#define glDetachShader			(*__vglDetachShader)
#define glDisableVertexAttribArray	(*__vglDisableVertexAttribArray)
#define glEnableVertexAttribArray	(*__vglEnableVertexAttribArray)
#define glGetActiveAttrib		(*__vglGetActiveAttrib)
#define glGetActiveUniform		(*__vglGetActiveUniform)
#define glGetAttachedShaders		(*__vglGetAttachedShaders)
#define glGetAttribLocation		(*__vglGetAttribLocation)
#define glGetProgramiv			(*__vglGetProgramiv)
#define glGetProgramInfoLog		(*__vglGetProgramInfoLog)
#define glGetShaderiv			(*__vglGetShaderiv)
#define glGetShaderInfoLog		(*__vglGetShaderInfoLog)
#define glGetShaderSource		(*__vglGetShaderSource)
#define glGetUniformLocation		(*__vglGetUniformLocation)
#define glGetUniformfv			(*__vglGetUniformfv)
#define glGetUniformiv			(*__vglGetUniformiv)
#define glGetVertexAttribdv		(*__vglGetVertexAttribdv)
#define glGetVertexAttribfv		(*__vglGetVertexAttribfv)
#define glGetVertexAttribiv		(*__vglGetVertexAttribiv)
#define glGetVertexAttribPointerv	(*__vglGetVertexAttribPointerv)
#define glIsProgram			(*__vglIsProgram)
#define glIsShader			(*__vglIsShader)
#define glLinkProgram			(*__vglLinkProgram)
#define glShaderSource			(*__vglShaderSource)
#define glUseProgram			(*__vglUseProgram)
#define glUniform1f			(*__vglUniform1f)
#define glUniform2f			(*__vglUniform2f)
#define glUniform3f			(*__vglUniform3f)
#define glUniform4f			(*__vglUniform4f)
#define glUniform1i			(*__vglUniform1i)
#define glUniform2i			(*__vglUniform2i)
#define glUniform3i			(*__vglUniform3i)
#define glUniform4i			(*__vglUniform4i)
#define glUniform1fv			(*__vglUniform1fv)
#define glUniform2fv			(*__vglUniform2fv)
#define glUniform3fv			(*__vglUniform3fv)
#define glUniform4fv			(*__vglUniform4fv)
#define glUniform1iv			(*__vglUniform1iv)
#define glUniform2iv			(*__vglUniform2iv)
#define glUniform3iv			(*__vglUniform3iv)
#define glUniform4iv			(*__vglUniform4iv)
#define glUniformMatrix2fv		(*__vglUniformMatrix2fv)
#define glUniformMatrix3fv		(*__vglUniformMatrix3fv)
#define glUniformMatrix4fv		(*__vglUniformMatrix4fv)
#define glValidateProgram		(*__vglValidateProgram)
#define glVertexAttrib1d		(*__vglVertexAttrib1d)
#define glVertexAttrib1dv		(*__vglVertexAttrib1dv)
#define glVertexAttrib1f		(*__vglVertexAttrib1f)
#define glVertexAttrib1fv		(*__vglVertexAttrib1fv)
#define glVertexAttrib1s		(*__vglVertexAttrib1s)
#define glVertexAttrib1sv		(*__vglVertexAttrib1sv)
#define glVertexAttrib2d		(*__vglVertexAttrib2d)
#define glVertexAttrib2dv		(*__vglVertexAttrib2dv)
#define glVertexAttrib2f		(*__vglVertexAttrib2f)
#define glVertexAttrib2fv		(*__vglVertexAttrib2fv)
#define glVertexAttrib2s		(*__vglVertexAttrib2s)
#define glVertexAttrib2sv		(*__vglVertexAttrib2sv)
#define glVertexAttrib3d		(*__vglVertexAttrib3d)
#define glVertexAttrib3dv		(*__vglVertexAttrib3dv)
#define glVertexAttrib3f		(*__vglVertexAttrib3f)
#define glVertexAttrib3fv		(*__vglVertexAttrib3fv)
#define glVertexAttrib3s		(*__vglVertexAttrib3s)
#define glVertexAttrib3sv		(*__vglVertexAttrib3sv)
#define glVertexAttrib4Nbv		(*__vglVertexAttrib4Nbv)
#define glVertexAttrib4Niv		(*__vglVertexAttrib4Niv)
#define glVertexAttrib4Nsv		(*__vglVertexAttrib4Nsv)
#define glVertexAttrib4Nub		(*__vglVertexAttrib4Nub)
#define glVertexAttrib4Nubv		(*__vglVertexAttrib4Nubv)
#define glVertexAttrib4Nuiv		(*__vglVertexAttrib4Nuiv)
#define glVertexAttrib4Nusv		(*__vglVertexAttrib4Nusv)
#define glVertexAttrib4bv		(*__vglVertexAttrib4bv)
#define glVertexAttrib4d		(*__vglVertexAttrib4d)
#define glVertexAttrib4dv		(*__vglVertexAttrib4dv)
#define glVertexAttrib4f		(*__vglVertexAttrib4f)
#define glVertexAttrib4fv		(*__vglVertexAttrib4fv)
#define glVertexAttrib4iv		(*__vglVertexAttrib4iv)
#define glVertexAttrib4s		(*__vglVertexAttrib4s)
#define glVertexAttrib4sv		(*__vglVertexAttrib4sv)
#define glVertexAttrib4ubv		(*__vglVertexAttrib4ubv)
#define glVertexAttrib4uiv		(*__vglVertexAttrib4uiv)
#define glVertexAttrib4usv		(*__vglVertexAttrib4usv)
#define glVertexAttribPointer		(*__vglVertexAttribPointer)

/* ARB versions */
#define glBlendEquationSeparateARB	(*__vglBlendEquationSeparate)
#define glDrawBuffersARB		(*__vglDrawBuffers)
#define glStencilOpSeparateARB		(*__vglStencilOpSeparate)
#define glStencilFuncSeparateARB	(*__vglStencilFuncSeparate)
#define glStencilMaskSeparateARB	(*__vglStencilMaskSeparate)
#define glAttachShaderARB		(*__vglAttachShader)
#define glBindAttribLocationARB		(*__vglBindAttribLocation)
#define glCompileShaderARB		(*__vglCompileShader)
#define glCreateProgramARB		(*__vglCreateProgram)
#define glCreateShaderARB		(*__vglCreateShader)
#define glDeleteProgramARB		(*__vglDeleteProgram)
#define glDeleteShaderARB		(*__vglDeleteShader)
#define glDetachShaderARB		(*__vglDetachShader)
#define glDisableVertexAttribArrayARB	(*__vglDisableVertexAttribArray)
#define glEnableVertexAttribArrayARB	(*__vglEnableVertexAttribArray)
#define glGetActiveAttribARB		(*__vglGetActiveAttrib)
#define glGetActiveUniformARB		(*__vglGetActiveUniform)
#define glGetAttachedShadersARB		(*__vglGetAttachedShaders)
#define glGetAttribLocationARB		(*__vglGetAttribLocation)
#define glGetProgramivARB		(*__vglGetProgramiv)
#define glGetProgramInfoLogARB		(*__vglGetProgramInfoLog)
#define glGetShaderivARB		(*__vglGetShaderiv)
#define glGetShaderInfoLogARB		(*__vglGetShaderInfoLog)
#define glGetShaderSourceARB		(*__vglGetShaderSource)
#define glGetUniformLocationARB		(*__vglGetUniformLocation)
#define glGetUniformfvARB		(*__vglGetUniformfv)
#define glGetUniformivARB		(*__vglGetUniformiv)
#define glGetVertexAttribdvARB		(*__vglGetVertexAttribdv)
#define glGetVertexAttribfvARB		(*__vglGetVertexAttribfv)
#define glGetVertexAttribivARB		(*__vglGetVertexAttribiv)
#define glGetVertexAttribPointervARB	(*__vglGetVertexAttribPointerv)
#define glIsProgramARB			(*__vglIsProgram)
#define glIsShaderARB			(*__vglIsShader)
#define glLinkProgramARB		(*__vglLinkProgram)
#define glShaderSourceARB		(*__vglShaderSource)
#define glUseProgramARB			(*__vglUseProgram)
#define glUniform1fARB			(*__vglUniform1f)
#define glUniform2fARB			(*__vglUniform2f)
#define glUniform3fARB			(*__vglUniform3f)
#define glUniform4fARB			(*__vglUniform4f)
#define glUniform1iARB			(*__vglUniform1i)
#define glUniform2iARB			(*__vglUniform2i)
#define glUniform3iARB			(*__vglUniform3i)
#define glUniform4iARB			(*__vglUniform4i)
#define glUniform1fvARB			(*__vglUniform1fv)
#define glUniform2fvARB			(*__vglUniform2fv)
#define glUniform3fvARB			(*__vglUniform3fv)
#define glUniform4fvARB			(*__vglUniform4fv)
#define glUniform1ivARB			(*__vglUniform1iv)
#define glUniform2ivARB			(*__vglUniform2iv)
#define glUniform3ivARB			(*__vglUniform3iv)
#define glUniform4ivARB			(*__vglUniform4iv)
#define glUniformMatrix2fvARB		(*__vglUniformMatrix2fv)
#define glUniformMatrix3fvARB		(*__vglUniformMatrix3fv)
#define glUniformMatrix4fvARB		(*__vglUniformMatrix4fv)
#define glValidateProgramARB		(*__vglValidateProgram)
#define glVertexAttrib1dARB		(*__vglVertexAttrib1d)
#define glVertexAttrib1dvARB		(*__vglVertexAttrib1dv)
#define glVertexAttrib1fARB		(*__vglVertexAttrib1f)
#define glVertexAttrib1fvARB		(*__vglVertexAttrib1fv)
#define glVertexAttrib1sARB		(*__vglVertexAttrib1s)
#define glVertexAttrib1svARB		(*__vglVertexAttrib1sv)
#define glVertexAttrib2dARB		(*__vglVertexAttrib2d)
#define glVertexAttrib2dvARB		(*__vglVertexAttrib2dv)
#define glVertexAttrib2fARB		(*__vglVertexAttrib2f)
#define glVertexAttrib2fvARB		(*__vglVertexAttrib2fv)
#define glVertexAttrib2sARB		(*__vglVertexAttrib2s)
#define glVertexAttrib2svARB		(*__vglVertexAttrib2sv)
#define glVertexAttrib3dARB		(*__vglVertexAttrib3d)
#define glVertexAttrib3dvARB		(*__vglVertexAttrib3dv)
#define glVertexAttrib3fARB		(*__vglVertexAttrib3f)
#define glVertexAttrib3fvARB		(*__vglVertexAttrib3fv)
#define glVertexAttrib3sARB		(*__vglVertexAttrib3s)
#define glVertexAttrib3svARB		(*__vglVertexAttrib3sv)
#define glVertexAttrib4NbvARB		(*__vglVertexAttrib4Nbv)
#define glVertexAttrib4NivARB		(*__vglVertexAttrib4Niv)
#define glVertexAttrib4NsvARB		(*__vglVertexAttrib4Nsv)
#define glVertexAttrib4NubARB		(*__vglVertexAttrib4Nub)
#define glVertexAttrib4NubvARB		(*__vglVertexAttrib4Nubv)
#define glVertexAttrib4NuivARB		(*__vglVertexAttrib4Nuiv)
#define glVertexAttrib4NusvARB		(*__vglVertexAttrib4Nusv)
#define glVertexAttrib4bvARB		(*__vglVertexAttrib4bv)
#define glVertexAttrib4dARB		(*__vglVertexAttrib4d)
#define glVertexAttrib4dvARB		(*__vglVertexAttrib4dv)
#define glVertexAttrib4fARB		(*__vglVertexAttrib4f)
#define glVertexAttrib4fvARB		(*__vglVertexAttrib4fv)
#define glVertexAttrib4ivARB		(*__vglVertexAttrib4iv)
#define glVertexAttrib4sARB		(*__vglVertexAttrib4s)
#define glVertexAttrib4svARB		(*__vglVertexAttrib4sv)
#define glVertexAttrib4ubvARB		(*__vglVertexAttrib4ubv)
#define glVertexAttrib4uivARB		(*__vglVertexAttrib4uiv)
#define glVertexAttrib4usvARB		(*__vglVertexAttrib4usv)
#define glVertexAttribPointerARB	(*__vglVertexAttribPointer)

#endif	// VGL_OPENGL2_0_EXTENSIONS

#ifndef VGL_OPENGL_ARB_SHADER_OBJECTS
#define VGL_OPENGL_ARB_SHADER_OBJECTS

/* This appears to be old Object style thingies */
#define glDeleteObjectARB		(*__vglDeleteObjectARB)
#define glGetHandleARB			(*__vglGetHandleARB)
#define glDetachObjectARB		(*__vglDetachObjectARB)
#define glCreateShaderObjectARB		(*__vglCreateShaderObjectARB)
#define glCreateProgramObjectARB	(*__glCreateProgramObjectARB)
#define glAttachObjectARB		(*__vglAttachObjectARB)
#define glUseProgramObjectARB		(*__vglUseProgramObjectARB)
#define glGetObjectParameterfvARB	(*__vglGetObjectParameterfvARB)
#define glGetObjectParameterivARB	(*__vglGetObjectParameterivARB)
#define glGetInfoLogARB			(*__vglGetInfoLogARB)
#define glGetAttachedObjectsARB		(*__vglGetAttachedObjectsARB)

#endif


#define glIsRenderbufferEXT		(*__vglIsRenderbufferEXT)
#define glBindRenderbufferEXT		(*__vglBindRenderbufferEXT)
#define glDeleteRenderbuffersEXT	(*__vglDeleteRenderbuffersEXT)
#define glGenRenderbuffersEXT		(*__vglGenRenderbuffersEXT)
#define glRenderbufferStorageEXT	(*__vglRenderbufferStorageEXT)
#define glGetRenderbufferParameterivEXT (*__vglGetRenderbufferParameterivEXT)
#define glIsFramebufferEXT		(*__vglIsFramebufferEXT)
#define glBindFramebufferEXT		(*__vglBindFramebufferEXT)
#define glDeleteFramebuffersEXT		(*__vglDeleteFramebuffersEXT)
#define glGenFramebuffersEXT		(*__vglGenFramebuffersEXT)
#define glCheckFramebufferStatusEXT	(*__vglCheckFramebufferStatusEXT)
#define glFramebufferTexture1DEXT	(*__vglFramebufferTexture1DEXT)
#define glFramebufferTexture2DEXT	(*__vglFramebufferTexture2DEXT)
#define glFramebufferTexture3DEXT	(*__vglFramebufferTexture3DEXT)
#define glFramebufferRenderbufferEXT	(*__vglFramebufferRenderbufferEXT)
#define glGetFramebufferAttachmentParameterivEXT	(*__vglGetFramebufferAttachmentParameterivEXT)
#define glGenerateMipmapEXT		(*__vglGenerateMipmapEXT)

extern PFNGLISRENDERBUFFEREXTPROC	__vglIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC	__vglBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC	__vglDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC	__vglGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC	__vglRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC	__vglGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC	__vglIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC	__vglBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC	__vglDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC	__vglGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	__vglCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC	__vglFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	__vglFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC	__vglFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	__vglFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	__vglGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC	__vglGenerateMipmapEXT;


extern PFNGLBLENDEQUATIONSEPARATEPROC		__vglBlendEquationSeparate;
extern PFNGLDRAWBUFFERSPROC			__vglDrawBuffers;
extern PFNGLSTENCILOPSEPARATEPROC		__vglStencilOpSeparate;
extern PFNGLSTENCILFUNCSEPARATEPROC		__vglStencilFuncSeparate;
extern PFNGLSTENCILMASKSEPARATEPROC		__vglStencilMaskSeparate;
extern PFNGLATTACHSHADERPROC			__vglAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC		__vglBindAttribLocation;
extern PFNGLCOMPILESHADERPROC			__vglCompileShader;
extern PFNGLCREATEPROGRAMPROC			__vglCreateProgram;
extern PFNGLCREATESHADERPROC			__vglCreateShader;
extern PFNGLDELETEPROGRAMPROC			__vglDeleteProgram;
extern PFNGLDELETESHADERPROC			__vglDeleteShader;
extern PFNGLDETACHSHADERPROC			__vglDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	__vglDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC		__vglEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC			__vglGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC		__vglGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC		__vglGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC		__vglGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC			__vglGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC		__vglGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC			__vglGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC		__vglGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC			__vglGetShaderSource;
extern PFNGLGETUNIFORMLOCATIONPROC		__vglGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC			__vglGetUniformfv;
extern PFNGLGETUNIFORMIVPROC			__vglGetUniformiv;
extern PFNGLGETVERTEXATTRIBDVPROC		__vglGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC		__vglGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC		__vglGetVertexAttribiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC		__vglGetVertexAttribPointerv;
extern PFNGLISPROGRAMPROC			__vglIsProgram;
extern PFNGLISSHADERPROC			__vglIsShader;
extern PFNGLLINKPROGRAMPROC			__vglLinkProgram;
extern PFNGLSHADERSOURCEPROC			__vglShaderSource;
extern PFNGLUSEPROGRAMPROC			__vglUseProgram;
extern PFNGLUNIFORM1FPROC			__vglUniform1f;
extern PFNGLUNIFORM2FPROC			__vglUniform2f;
extern PFNGLUNIFORM3FPROC			__vglUniform3f;
extern PFNGLUNIFORM4FPROC			__vglUniform4f;
extern PFNGLUNIFORM1IPROC			__vglUniform1i;
extern PFNGLUNIFORM2IPROC			__vglUniform2i;
extern PFNGLUNIFORM3IPROC			__vglUniform3i;
extern PFNGLUNIFORM4IPROC			__vglUniform4i;
extern PFNGLUNIFORM1FVPROC			__vglUniform1fv;
extern PFNGLUNIFORM2FVPROC			__vglUniform2fv;
extern PFNGLUNIFORM3FVPROC			__vglUniform3fv;
extern PFNGLUNIFORM4FVPROC			__vglUniform4fv;
extern PFNGLUNIFORM1IVPROC			__vglUniform1iv;
extern PFNGLUNIFORM2IVPROC			__vglUniform2iv;
extern PFNGLUNIFORM3IVPROC			__vglUniform3iv;
extern PFNGLUNIFORM4IVPROC			__vglUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC		__vglUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC		__vglUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC		__vglUniformMatrix4fv;
extern PFNGLVALIDATEPROGRAMPROC			__vglValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC			__vglVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC			__vglVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC			__vglVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC			__vglVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC			__vglVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC			__vglVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC			__vglVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC			__vglVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC			__vglVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC			__vglVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC			__vglVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC			__vglVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC			__vglVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC			__vglVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC			__vglVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC			__vglVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC			__vglVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC			__vglVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC		__vglVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC		__vglVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC		__vglVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC		__vglVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC		__vglVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC		__vglVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC		__vglVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC			__vglVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC			__vglVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC			__vglVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC			__vglVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC			__vglVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC			__vglVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC			__vglVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC			__vglVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC		__vglVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC		__vglVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC		__vglVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC		__vglVertexAttribPointer;


extern PFNGLDELETEOBJECTARBPROC			__vglDeleteObjectARB;
extern PFNGLGETHANDLEARBPROC			__vglGetHandleARB;
extern PFNGLDETACHOBJECTARBPROC			__vglDetachObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC		__vglCreateShaderObjectARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC		__glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC			__vglAttachObjectARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC		__vglUseProgramObjectARB;
extern PFNGLGETOBJECTPARAMETERFVARBPROC		__vglGetObjectParameterfvARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC		__vglGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC			__vglGetInfoLogARB;
extern PFNGLGETATTACHEDOBJECTSARBPROC		__vglGetAttachedObjectsARB;


#ifdef __cplusplus
extern "C" {
#endif

    int vglInit(FILE * out);

#ifdef __cplusplus
}
#endif

#endif /* _vgl_h_ */
