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
#include "../inc/iv_gles_wrapper.h"
//=================================================================
namespace iv
{
/**
 * @brief Constructor : Initializes requested
 *
 * @param rc RenderConfig
 */
GLESWrapper::GLESWrapper() //: _GLESWrapperThread(boost::bind(&GLESWrapper::drawFrames, this))
{

}
//=================================================================
GLESWrapper::~GLESWrapper()
{

}
//=================================================================
void GLESWrapper::startRender()
{
    _GLESWrapperThread = new boost::thread(boost::bind(&GLESWrapper::eglMainLoop, this));
    _GLESWrapperThread->join();
    //eglMainLoop ();
}
//=================================================================
EGLint GLESWrapper::initEGL(RenderConfig rc)
{
    _u32width = rc.u32width;
    _u32height = rc.u32height;
    _u32flags = rc.eflags;
    _pu8windowTitle = rc.pu8windowTitle;
    std::cerr<<"In GLESWrapper Constructor.... "<<_u32width<<" "
                                            <<_u32height<<" "
                                           <<_u32flags<<std::endl;
    createWindow();
}
//=================================================================
EGLint GLESWrapper::createWindow()
{
    createWindowX11();
    createEGLContext();
}
}
 //End of namespcae vi
