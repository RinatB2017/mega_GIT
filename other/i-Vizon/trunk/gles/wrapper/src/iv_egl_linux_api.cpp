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
/**
* =============================================================================
*
*
*   FILENAME            : GLESWrapper.cpp
*
*   DESCRIPTION         : Acquires frames from two camer and time stamps it.
*                         Deques the frames respective to message type and
*                         sends to respective attached observers
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                    AUTHOR	                DESCRIPTION
*   07th Feb 2014  	        	                    File Created.
*
* =============================================================================
*/
//Reference http://wiki.maemo.org/SimpleGL_example

#include "../inc/iv_gles_wrapper.h"

//=================================================================
//  EGL Native Window Headers
//=================================================================
#ifdef EGL_LINUX
//=================================================================
//  Linux Native Headers
//=================================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#else
//=================================================================
//  Windows Native Headers
//=================================================================

#endif

namespace iv
{
/**
 * @brief Creates a native window using X11
 *
 * @return EGLBoolean
 */
EGLBoolean GLESWrapper::createWindowX11()
{
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wm_state;
    XWMHints hints;
    XEvent xev;
    Window win;

    /*
     * X11 native display initialization
     */

    x_display = XOpenDisplay(NULL);
    if ( x_display == NULL )
    {
        return EGL_FALSE;
    }

    // get the root window (usually the whole screen)
    root = DefaultRootWindow(x_display);

    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(
                x_display, root,
                0, 0, _u32width, _u32height, 0,
                CopyFromParent, InputOutput,
                CopyFromParent, CWEventMask,
                &swa );

    xattr.override_redirect = false;
    XChangeWindowAttributes ( x_display, win, CWOverrideRedirect, &xattr );

    hints.input = true;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
    XMapWindow (x_display, win);
    XStoreName (x_display, win, _pu8windowTitle);

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (x_display, "_NET_WM_STATE", false);

    memset ( &xev, 0, sizeof(xev) );
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = false;
    XSendEvent (
                x_display,
                DefaultRootWindow ( x_display ),
                false,
                SubstructureNotifyMask,
                &xev );

    _u64nativeWndHandle = (EGLNativeWindowType) win;
    return EGL_TRUE;
}
//=================================================================
EGLBoolean GLESWrapper::createEGLContext ()
{
    EGLint u32numConfigs;
    EGLint u32majorVersion;
    EGLint u32minorVersion;
    EGLConfig config;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (_u32flags & MD_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (_u32flags & MD_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (_u32flags & MD_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (_u32flags & MD_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_NONE
    };

    // Get Display
    _v0eglDisplay = eglGetDisplay((EGLNativeDisplayType)x_display);
    if ( _v0eglDisplay == EGL_NO_DISPLAY )
    {
        return EGL_FALSE;
    }

    // Initialize EGL
    if ( !eglInitialize(_v0eglDisplay, &u32majorVersion, &u32minorVersion) )
    {
        return EGL_FALSE;
    }
    std::cout<<"u32majorVersion"<<u32majorVersion<<std::endl;
    std::cout<<"u32minorVersion"<<u32minorVersion<<std::endl;
    // Get configs
    if ( !eglGetConfigs(_v0eglDisplay, NULL, 0, &u32numConfigs) )
    {
        return EGL_FALSE;
    }

    // Choose config
    if ( !eglChooseConfig(_v0eglDisplay, attribList, &_v0eglConfig, 1, &u32numConfigs) )
    {
        return EGL_FALSE;
    }

    eglBindAPI(EGL_OPENGL_ES_API);
    // Create a surface
    _v0eglSurface = eglCreateWindowSurface(_v0eglDisplay, _v0eglConfig, (EGLNativeWindowType)_u64nativeWndHandle, NULL);
    if ( _v0eglSurface == EGL_NO_SURFACE )
    {
        return EGL_FALSE;
    }

    // Create a GL context
    _v0eglContext = eglCreateContext(_v0eglDisplay, _v0eglConfig, EGL_NO_CONTEXT, contextAttribs );
    if ( _v0eglContext == EGL_NO_CONTEXT )
    {
        return EGL_FALSE;
    }

    // Make the context current
    if ( !eglMakeCurrent(_v0eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, _v0eglContext) )
    {
          return EGL_FALSE;
    }

    std::cout<<"\n\n\n_v0eglDisplay\t"<<_v0eglDisplay<<std::endl;
    std::cout<<"\n\n\n\n_v0eglSurface"<<_v0eglSurface<<std::endl;
    return EGL_TRUE;
}

//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application
//
EGLBoolean GLESWrapper::eglMainLoop ()
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;
    float totaltime = 0.0f;
    unsigned int frames = 0;
    EGLContext volocalEglContext;
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

    gettimeofday ( &t1 , &tz );

    // Create a EGL local context
    volocalEglContext = eglCreateContext(_v0eglDisplay, _v0eglConfig, _v0eglContext, contextAttribs );
    if ( _v0eglContext == EGL_NO_CONTEXT )
    {
        return EGL_FALSE;
    }

    // Make the context current
    if ( !eglMakeCurrent(_v0eglDisplay, _v0eglSurface, _v0eglSurface, volocalEglContext) )
    {

        std::cerr<<"eglMakecurrent failed\n";
        std::cerr<<"EGL error :\t"<<getEGLErrorString(eglGetError());
        return EGL_FALSE;
    }

    while(userInterrupt() == GL_FALSE)
    {
        gettimeofday(&t2, &tz);
        deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;

        drawFrames();
        std::cout<<"\n_v0eglDisplay\t"<<_v0eglDisplay<<std::endl;
        std::cout<<"_v0eglSurface\t"<<_v0eglSurface<<std::endl;
        if(!eglSwapBuffers(_v0eglDisplay, _v0eglSurface))
        {
            std::cout<<"egl swap failed.......\n\n";
            std::cout<<"_v0eglDisplay\t"<<_v0eglDisplay<<std::endl;
            std::cout<<"_v0eglSurface"<<_v0eglSurface<<std::endl;
            std::cout<<"EGL error :\t"<<getEGLErrorString(eglGetError());
        }

        totaltime += deltatime;
        frames++;
        if (totaltime >  2.0f)
        {
            //printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, frames/totaltime);
            totaltime -= 2.0f;
            frames = 0;
        }
    }
}
//userInterrupt()
//
//Reads from X11 event loop and interrupt program if there is a keypress, or
//window close action.
//
GLboolean GLESWrapper::userInterrupt()
{
    XEvent xev;
    KeySym key;
    GLboolean userinterrupt = GL_FALSE;
    char text;

    // Pump all messages from X server. Keypresses are directed to keyfunc (if defined)
    while ( XPending ( x_display ) )
    {
        XNextEvent( x_display, &xev );
        if ( xev.type == KeyPress )
        {
            if (XLookupString(&xev.xkey,&text,1,&key,0)==1)
            {
                //                if (esContext->keyFunc != NULL)
                //                    esContext->keyFunc(esContext, text, 0, 0);
            }
        }
        if ( xev.type == DestroyNotify )
            userinterrupt = GL_TRUE;
    }
    return userinterrupt;
}

//=================================================================
/**
 * @brief Return EGL error string corresponding to the errNO
 *
 * @param errNo: Return value of eglGetError()
 * @return const char : Returns an EGL error string
 */
const char *GLESWrapper::getEGLErrorString(EGLint errNo)
{
    switch(errNo)
    {
    case 0x3000:
        return "EGL_SUCCESS";
        break;
    case 0x3001:
        return "EGL_NOT_INITIALIZED";
        break;
    case  0x3002:
        return "EGL_BAD_ACCESS";
        break;
    case 0x3003:
        return "EGL_BAD_ALLOC"	;
        break;
    case 0x3004:
        return "EGL_BAD_ATTRIB//EGLint EGLHandle::_cu32maxNoConfigs = 10;UTE";
        break;
    case 0x3005:
        return "EGL_BAD_CONFIG";
        break;
    case 0x3006:
        return "EGL_BAD_CONTEXT";
        break;
    case 0x3007:
        return "EGL_BAD_CURRENT_SURFACE";
        break;
    case 0x3008:
        return "EGL_BAD_DISPLAY";
        break;
    case 0x3009:
        return "EGL_BAD_MATCH";
        break;
    case 0x300A:
        return "EGL_BAD_NATIVE_PIXMAP";
        break;
    case 0x300B:
        return "EGL_BAD_NATIVE_WINDOW";
        break;
    case 0x300C:
        return "EGL_BAD_PARAMETER";
        break;
    case 0x300D:
        return "EGL_BAD_SURFACE";
        break;
    case 0x300E:
        return "EGL_CONTEXT_LOST";			/* EGL 1.1 - IMG_power_management */
        break;
    default:
        return "Not an EGL Error";
        break;
    }
}

} //End of namespace vi

