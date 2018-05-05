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
#ifndef MATRIXOPERATIONS_H
#define MATRIXOPERATIONS_H


/*****************************************
 *  List of Matrix Operations
 *  1.Matrix Scaling
 *  2.Matrix Translate
 *  3.Matrix Rotational
 *  4.Matrix Frustum
 *  5.Orthogonal Matrix
 *  6.Perspective Matrix
 *  7.MatrixMultiply
 *  8.Identity Matrix
 *  9.
 *
 ******************************************/

/******************************************
 *  Matrix 4 * 3
 ******************************************/

//==============================================================
//GLES Headers & Macros
//==============================================================
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

#include <iv_common.h>
namespace iv
{

#define PI 3.1415926535897932384626433832795f
typedef enum
{
    MATRIX_4_4 = 4
}MATRIX_DIMENSION;
typedef struct
{
    GLfloat m[4][4];
}Matrix;

void LoadIdentity(Matrix *result);
void Perspective(Matrix *result, float fovy, float aspect, float nearZ, float farZ);
void Frustum(Matrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);
void Translate(Matrix *result, GLfloat tx, GLfloat ty, GLfloat tz);
void Rotate(Matrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void MatrixMultiply(Matrix *result, Matrix *srcA, Matrix *srcB);
}
#endif // MATRIXOPERATIONS_H
