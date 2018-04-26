/*
****************************************************************************
BSD License
Copyright (c) 2014, i-Vizon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the i-Vizon.
4. Neither the name of the i-Vizon nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY Mageswaran.D ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL Mageswaran.D BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

****************************************************************************
*/
/*
* =============================================================================
*
*
*   FILENAME            : GLESWrapper.h
*
*   DESCRIPTION         : A wrapper library for EGL and its native counter part
*                         intialization. With boost thread support.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 20th Feb 2014
*
*   File Created.
*
* =============================================================================
*/


#ifndef _IMG_RENDER_H_
#define _IMG_RENDER_H_

#include <iostream>

//==============================================================
//EGL Headers & Macros
//==============================================================
#include <EGL/egl.h>
#include <EGL/eglplatform.h>

//==============================================================
//OpenCV Headers & Macros
//==============================================================
//#include <opencv/highgui.h>

//==============================================================
//GLES Headers & Macros
//==============================================================
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

//==============================================================
//  Boost Native Headers
//==============================================================
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

namespace iv
{
//==============================================================
/**
 * @brief To handle different target window surface for rendering
 *        EGL_SURFACE : Configures OpenGLES to use EGL Framework
 *        QT_SURFACE  : Configures OpenGLES to use Qt Framework
 *        USER_SURFACE: Configures OpenGLES to use User Framework
 */
typedef enum _renderSurface
{
    EGL_SURFACE = 0,
    QT_SURFACE,
    USER_SURFACE
} RENDER_SURFACE;

//=================================================================
typedef enum _windowFlags
{
    /// esCreateWindow flag - RGB color buffer
    MD_WINDOW_RGB= 0,
    /// esCreateWindow flag - ALPHA color buffer
    MD_WINDOW_ALPHA=1,
    /// esCreateWindow flag - depth buffer
    MD_WINDOW_DEPTH=2,
    /// esCreateWindow flag - stencil buffer
    MD_WINDOW_STENCIL=4,
    /// esCreateWindow flat - multi-sample buffer
    MD_WINDOW_MULTISAMPLE=8
}WINDOW_FLAGS;

//=================================================================
/**
 * @brief Configuration settings for render
 *
 */
typedef struct _renderConfig
{
    RENDER_SURFACE eRenderSurface;
    GLint u32width;
    GLint u32height;
    WINDOW_FLAGS eflags;
    const char *pu8windowTitle;
} RenderConfig;

//=================================================================
/**
 * @brief Renders the observed frame data to the configured surface.
 *        Surface can be from EGL / Qt Surface / User specified one.
 *
 */
class GLESWrapper
{

public:
    GLESWrapper();

    virtual ~GLESWrapper();

    void startRender();

    //=================================================================
    //  EGL Artributes
    //=================================================================
    //=================================================================
    EGLint initEGL(RenderConfig rc);
    //=================================================================
    //  GLES Artributes
    //=================================================================
    //=================================================================
    ///Method to clear the EGL surface and make it ready for
    ///GL ES to draw.
    ///Needs to be implemented by each example
    virtual GLint initGLES() = 0;
    //=================================================================
    ///Method to put graphics content in each frame.
    ///Needs to be implemented by each example.
    virtual void drawFrames() = 0;

protected:
    //=================================================================
    //  GLES Variables and Artributes
    //=================================================================
    GLint _u32width;
    GLint _u32height;
    GLint _u32flags;
    GLuint _u32programObject; // Handle to a program object

    //=================================================================
    ///Load Shader
    GLuint LoadShader ( GLenum type, const char *shaderSrc );
    //=================================================================
    ///Load Shader File
    GLuint LoadShaderFile(GLenum Type,std::string filename);
    //=================================================================
    ///Get Program Object
    GLuint getGLProgramID(std::string vertexFilename,
                          std::string fragmentFilename);

private:
    boost::thread *_GLESWrapperThread;
    //=================================================================
    //  EGL Variables and Artributes
    //=================================================================
    Display *x_display; // X11 related local variables
    const char *_pu8windowTitle;

    EGLNativeWindowType  _u64nativeWndHandle;

    EGLint _u32eglConfig;
    EGLint _cu32maxNoConfigs;

    EGLConfig _v0eglConfig;

    EGLDisplay _v0eglDisplay;
    EGLContext _v0eglContext;
    EGLSurface _v0eglSurface;

    //=================================================================
    EGLint createWindow();
    //=================================================================
    EGLBoolean createWindowX11();
    //=================================================================
    EGLBoolean createEGLContext ();
    //=================================================================
    EGLBoolean eglMainLoop();
    //=================================================================
    GLboolean userInterrupt();
    //=================================================================
    const char* getEGLErrorString(EGLint errNo);
};

} //End of namespaace vi
#endif
