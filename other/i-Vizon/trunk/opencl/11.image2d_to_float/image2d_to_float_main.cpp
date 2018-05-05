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
*   FILENAME            : test_image_add.cpp
*
*   DESCRIPTION         : A test code for to understand the image data type
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

#include <cl_wrapper.hpp>
#include <cv_wrapper.h>

using namespace std;

int main()
{
    cout << "Image2d to Float Conversion" << endl;
    iv::CLSetup cl;
    iv::Program* prog;
    iv::KernelLauncher* kl;
    std::string file1 = "image2d_to_float.cl";
    std::vector<std::string> filesPath;
    filesPath.push_back(file1);

    cl.init();
    prog = cl.createProgram(filesPath);
    prog->buildProgram();
    kl = prog->createKernelLauncher("image2d_to_float");

    int width, height, step;
    uchar* pixelPointer;
    std::string image1Name = "shiva46x46.jpg";
    iv::CVSetup cv;
    cv.openImageBGRA(image1Name, width, height,
                     step, pixelPointer);
    cv.showImage();
    return 0;
}

