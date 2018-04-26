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
*   FILENAME            : GLESImpl.cpp
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

#include <iostream>
#include <fstream>
#include <iostream>
#include <iterator>
namespace iv
{
GLuint GLESWrapper::LoadShaderFile(GLenum type,  std::string filename)
{
        GLuint shader;

        std::ifstream inputFileStream(filename.c_str());
        if(!inputFileStream)
            throw(std::runtime_error("File:"+filename+" not opened."));
        std::ostringstream stream;
        stream<<inputFileStream.rdbuf();

        std::string dataString = stream.str();
        const GLchar *data = reinterpret_cast<const GLchar*>(dataString.c_str());

        shader = glCreateShader(type);
        glShaderSource(shader,1,static_cast<const GLchar**>(&data),0);

        glCompileShader(shader);

        int status;
        glGetShaderiv(shader,GL_COMPILE_STATUS,&status);
        if(status == GL_FALSE)
        {
            int loglength;
            glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&loglength);

            char *data = new char[loglength];

            glGetShaderInfoLog(shader,loglength,&loglength,data);

            std::string strdata(data);
            delete [] data;

            throw(std::runtime_error(strdata));
            exit(0);
        }
        return shader;
}




//=================================================================
GLuint GLESWrapper::LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
        return 0;

    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled )
    {
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            //esLogMessage ( "Error compiling shader:\n%s\n", infoLog );
            free ( infoLog );
        }

        glDeleteShader ( shader );
        return 0;
    }
    return shader;
}

GLuint GLESWrapper::getGLProgramID(std::string vertexFilename, std::string fragmentFilename)
{
    GLuint vertexShader;
    GLuint fragmentShader1;
    GLuint programObject;
    GLint linked;

    fragmentShader1 = LoadShaderFile(GL_FRAGMENT_SHADER,fragmentFilename );
    vertexShader = LoadShaderFile(GL_VERTEX_SHADER, vertexFilename);

    programObject = glCreateProgram();
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader1);
    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if ( !linked )
    {
        GLint infoLen = 0;
        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );
            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            free ( infoLog );
        }
        glDeleteProgram ( programObject );
        return GL_FALSE;
    }
    return programObject;
}
}//End of namespace vi
