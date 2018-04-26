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
*   FILENAME            : cv_wrapper.cpp
*
*   DESCRIPTION         : A simple wrapper to read image files and to access
*                         camera to get raw pixels.
*
*   AUTHOR              : Mageswaran D
*
*
*   CHANGE HISTORY      :
*
*   DATE                : 22th April 2014
*
*   File Created.
*
* =============================================================================
*/
#include "../inc/cv_wrapper.h"

namespace iv
{

void CVSetup::openCamera(int id)
{
}

void CVSetup::openImageRGBA(std::string filePath, int &width, int &height, int &step, uchar* pixelPointer)
{
}

void CVSetup::openImageBGRA(std::string filePath, int &width, int &height, int &step, uchar* &pixelPointer)
{
    _matrixHolder = cv::imread(filePath);
    cv::cvtColor(_matrixHolder, _matrixHolder, CV_BGR2BGRA);
    width   =   _matrixHolder.size().width;
    height  =   _matrixHolder.size().height;
    step    =   _matrixHolder.step;
    pixelPointer = _matrixHolder.data;

    DEBUG_VALUE("Image Width: ", width);
    DEBUG_VALUE("Image Height: ", height);
    DEBUG_VALUE("Image Step: ", step);
}

void CVSetup::showImage()
{
    cv::imshow("Original Image", _matrixHolder);
    cv::waitKey();
}

} //end of namespace
